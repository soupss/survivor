#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tank.h"
#include <raymath.h>
#include "util.h"
#include "status.h"

void _tank_shoot(Tank *t, List *bs, SoundEffects *sfx);
void _tank_hp_regenerate(Tank *t);
void _tank_contain_in_map(Tank *t);

struct Tank {
    Rectangle rec;
    Vector2 hull_dir;
    Vector2 velocity;
    float turret_radius;
    Vector2 turret_dir;
    int hit_points;
    StatusKnockback *status_knockback;
    StatusRecoil *status_recoil;
    int hurt_delta;
};

Tank *tank_create(int x, int y) {
    Tank *t = malloc(sizeof(Tank));
    util_check_alloc(t);
    t->rec = (Rectangle){x, y, TANK_HULL_WIDTH, TANK_HULL_HEIGHT};
    t->hull_dir = (Vector2){0, 1};
    t->velocity = (Vector2){0};
    t->turret_radius = TANK_TURRET_RADIUS;
    t->turret_dir = (Vector2){0, 1};
    t->hit_points = TANK_MAX_HP;
    t->status_knockback = NULL;
    t->status_recoil = NULL;
    t->hurt_delta = 0;
    return t;
}

void tank_destroy(Tank *t) {
    //TODO: explosion
    free(t);
}

#define TANK_MOVE_SPEED 2
void tank_update(Tank *t, List *bs, List *ms, SoundEffects *sfx) {
    if (Vector2Length(t->velocity) > 0) {
        t->rec.x += t->velocity.x * TANK_MOVE_SPEED;
        t->rec.y += t->velocity.y * TANK_MOVE_SPEED;
    }
    Vector2 separation = util_separation_from_mobs(t->rec.x, t->rec.y, TANK_HITBOX_RADIUS, ms, 0.5);
    t->rec.x += separation.x * TANK_MOVE_SPEED;
    t->rec.y += separation.y * TANK_MOVE_SPEED;
    _tank_contain_in_map(t);
    if (t->status_knockback != NULL)
        status_knockback_update(&t->status_knockback);
    if (t->status_knockback != NULL) {
        Vector2 kb = status_knockback_get(t->status_knockback);
        t->rec.x += kb.x;
        t->rec.y += kb.y;
    }
    if (t->status_recoil != NULL)
        status_recoil_update(&t->status_recoil);
    _tank_shoot(t, bs, sfx);
    _tank_hp_regenerate(t);
    t->hurt_delta++;
}

#define TANK_HULL_COLOR DARKGREEN
#define TANK_TRACK_COLOR DARKGRAY
#define TANK_TURRET_COLOR GREEN
void tank_draw(Tank *t) {
    Vector2 hull_center = {t->rec.width / 2, t->rec.height / 2};
    float hull_rotation = -RAD2DEG * Vector2Angle(t->hull_dir, (Vector2){0, -1});
    DrawRectanglePro(t->rec, hull_center, hull_rotation, TANK_HULL_COLOR);
    Rectangle track_rec = {t->rec.x, t->rec.y, TANK_TRACK_WIDTH, t->rec.height};
    DrawRectanglePro(track_rec, hull_center, hull_rotation, TANK_TRACK_COLOR);
    DrawRectanglePro(track_rec, hull_center, 180 + hull_rotation, TANK_TRACK_COLOR);
    Vector2 turret_pos = Vector2Subtract((Vector2){t->rec.x, t->rec.y}, Vector2Scale(t->hull_dir, t->rec.height / 2 - t->rec.width / 2));
    DrawCircleV(turret_pos, t->turret_radius, TANK_TURRET_COLOR);
    float recoil = status_recoil_get(t->status_recoil);
    float barrel_length = TANK_BARREL_MAX_LENGTH - recoil;
    Vector2 barrel_pos = Vector2Add(turret_pos, Vector2Scale(t->turret_dir, t->turret_radius + barrel_length / 2 - TANK_TURRET_RADIUS * 0.05));
    Rectangle barrel_rec = {barrel_pos.x, barrel_pos.y, TANK_BARREL_WIDTH, barrel_length};
    Vector2 barrel_center = {TANK_BARREL_WIDTH / 2, barrel_length / 2};
    float turret_rotation = -RAD2DEG * Vector2Angle(t->turret_dir, (Vector2){0, -1});
    DrawRectanglePro(barrel_rec, barrel_center, turret_rotation, TANK_TURRET_COLOR);
    DrawFPS(1, 0);
}

#define TANK_MOVE_ACC 0.03
#define TANK_MOVE_DEC 0.03
#define TANK_VEL_ROT_STAB_FACTOR 0.2
void tank_velocity_calculate(Tank *t, int dir) {
    int dir_vel = 0;
    if (Vector2Length(t->velocity) != 0){
        float dot = Vector2DotProduct(t->velocity, t->hull_dir);
        if (dot >= 0)
            dir_vel = 1;
        else if (dot < 0)
            dir_vel = -1;
    }
    if (dir != 0) {
        // start from stop
        if (dir_vel == 0) {
            float magnitude = TANK_MOVE_ACC;
            float angle = -Vector2Angle(t->hull_dir, (Vector2){1, 0});
            if (dir == -1) {
                magnitude = TANK_MOVE_DEC;
                angle += PI;
            }
            t->velocity = Vector2Rotate((Vector2){magnitude, 0}, angle);
        }
        else {
            // accelerate
            if (dir_vel == dir)
                t->velocity = Vector2AddMagnitude(t->velocity, TANK_MOVE_ACC);
            // decelerate to change direction
            else
                t->velocity = Vector2SubtractMagnitude(t->velocity, TANK_MOVE_DEC + TANK_MOVE_ACC);
        }
    }
    // decelerate
    else {
        t->velocity = Vector2SubtractMagnitude(t->velocity, TANK_MOVE_DEC);
    }
    t->velocity = Vector2ClampValue(t->velocity, 0, 1);
    Vector2 dir_target = t->hull_dir;
    if (dir_vel == -1)
        dir_target = Vector2Rotate(dir_target, PI);
    float diff = Vector2Angle(t->velocity, dir_target);
    if (FloatEquals(diff, 0) == 0)
        t->velocity = Vector2Rotate(t->velocity, TANK_VEL_ROT_STAB_FACTOR*diff);
}

#define TANK_HULL_ROT_SPEED 0.035
#define TANK_HULL_ROT_ACC 0.07
#define TANK_HULL_ROT_DEC 0.14
void tank_hull_rotate(Tank *t, int dir) {
    static int dir_curr = 0;
    static float momentum = 0;
    if (dir != dir_curr) {
        if (momentum > 0.0) {
            momentum -= TANK_HULL_ROT_DEC + TANK_HULL_ROT_ACC;
            if (momentum < 0) {
                momentum = 0;
                dir_curr = dir;
            }
        }
        else
            dir_curr = dir;
    }
    else {
        if (dir != 0 && momentum < 1.0) {
            momentum += TANK_HULL_ROT_ACC;
            if (momentum > 1.0)
                momentum = 1.0;
        }
        else if (dir == 0 && momentum > 0.0) {
            momentum -= TANK_HULL_ROT_DEC;
            if (momentum < 0) {
                momentum = 0;
                dir_curr = 0;
            }
        }
    }
    if (momentum > 0.0)
        t->hull_dir = Vector2Rotate(t->hull_dir, momentum * dir_curr * TANK_HULL_ROT_SPEED);
}

#define TANK_TURRET_ROT_SPEED 0.08
#define TANK_TURRET_ROT_ACC 0.05
#define TANK_TURRET_ROT_DEC 0.12
void tank_turret_rotate(Tank *t, int dir) {
    static int dir_curr = 0;
    static float momentum = 0;
    if (dir != 0 && dir != dir_curr) {
        momentum = 0;
        dir_curr = dir;
    }
    if (dir != 0 && momentum < 1.0) {
        momentum += TANK_TURRET_ROT_ACC;
        if (momentum > 1.0)
            momentum = 1.0;
    }
    else if (dir == 0 && momentum > 0.0) {
        momentum -= TANK_TURRET_ROT_DEC;
        if (momentum < 0) {
            momentum = 0;
            dir_curr = 0;
        }
    }
    if (momentum > 0.0)
        t->turret_dir = Vector2Rotate(t->turret_dir, momentum * dir_curr * TANK_TURRET_ROT_SPEED);
}

#define TANK_SHOOT_DELAY 40
#define TANK_SHOOT_RECOIL_KICKBACK_FACTOR 0.5
#define TANK_SHOOT_RECOIL_DURATION_FACTOR 0.5
#define TANK_SHOOT_RECOIL_DURATION_FACTOR2 0.5
void _tank_shoot(Tank *t, List *bs, SoundEffects *sfx) {
    static int shot_delta = 0;
    if (shot_delta >= TANK_SHOOT_DELAY) {
        PlaySound(sfx->shoot);
        Vector2 turret_pos = Vector2Subtract((Vector2){t->rec.x, t->rec.y}, Vector2Scale(t->hull_dir, t->rec.height / 2 - t->rec.width / 2));
        Vector2 barrel_end = Vector2Add(turret_pos, Vector2Scale(t->turret_dir, t->turret_radius + TANK_BARREL_MAX_LENGTH - TANK_TURRET_RADIUS * 0.05 - BULLET_RADIUS * 2));
        Bullet *b = bullet_create(barrel_end, t->turret_dir);
        list_insert(bs, b);
        shot_delta = 0;
        float r_kickback = TANK_SHOOT_RECOIL_KICKBACK_FACTOR * TANK_BARREL_MAX_LENGTH;
        float dmg = bullet_get_damage(b);
        //TODO: math
        float r_duration = TANK_SHOOT_RECOIL_DURATION_FACTOR * TANK_SHOOT_DELAY + TANK_SHOOT_RECOIL_DURATION_FACTOR2 * dmg; // depend on dmg
        t->status_recoil = status_recoil_create(r_kickback, r_duration);
    }
    else
        shot_delta++;
}

#define TANK_HEALTH_REGEN_DELAY 200
#define TANK_HEALTH_REGEN_TICK_DELAY 30
#define TANK_HEALTH_REGEN_AMOUNT 1
void _tank_hp_regenerate(Tank *t) {
    static int regen_tick_delta = TANK_HEALTH_REGEN_TICK_DELAY;
    if (t->hurt_delta >= TANK_HEALTH_REGEN_DELAY && t->hit_points < TANK_MAX_HP) {
        if (regen_tick_delta >= TANK_HEALTH_REGEN_TICK_DELAY) {
            t->hit_points += TANK_HEALTH_REGEN_AMOUNT;
            regen_tick_delta = 0;
        }
        else
            regen_tick_delta++;
    }
    else
        regen_tick_delta = TANK_HEALTH_REGEN_TICK_DELAY;
}

void _tank_contain_in_map(Tank *t) {
    float r = TANK_HITBOX_RADIUS;
    if (t->rec.x - r < 0)
        t->rec.x = r;
    else if (t->rec.x + r > SCREEN_WIDTH)
        t->rec.x = SCREEN_WIDTH - r;
    else if (t->rec.y - r < 0)
        t->rec.y = r;
    else if (t->rec.y + r > SCREEN_HEIGHT)
        t->rec.y = SCREEN_HEIGHT - r;
}

void tank_hp_reduce(Tank *t, int hp) {
    t->hit_points -= hp;
    t->hurt_delta = 0;
    if (t->hit_points < 0)
        t->hit_points = 0;
}

void tank_set_status_knockback(Tank *t, float distance, float angle, int duration) {
   t->status_knockback = status_knockback_create(distance, angle, duration);
}

bool tank_is_dead(Tank *t) {
    if (t->hit_points <= 0)
        return true;
    else
        return false;
}

int tank_get_hp(Tank *t) {
    return t->hit_points;
}

Vector2 tank_get_pos(Tank *t) {
    return (Vector2){t->rec.x, t->rec.y};
}
