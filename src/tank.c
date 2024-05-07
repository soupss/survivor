#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tank.h"
#include <raymath.h>
#include "util.h"
#include "status.h"
#include "weapon.h"

void _tank_shoot(Tank *t, List *bs, SoundEffects *sfx);
void _tank_hp_regenerate(Tank *t);
void _tank_contain_in_map(Tank *t);
Vector2 _tank_get_turret_pos(Tank *t);

struct Tank {
    Rectangle rec;
    List *weapons;
    Vector2 velocity;
    Vector2 hull_dir;
    Vector2 turret_dir;
    int hit_points;
    int hurt_delta;
    StatusKnockback *status_knockback;
    StatusRecoil *status_recoil;
};

Tank *tank_create(int x, int y) {
    Tank *t = malloc(sizeof(Tank));
    util_check_alloc(t);
    t->rec = (Rectangle){x, y, TANK_BASE_WIDTH, TANK_BASE_HEIGHT};
    t->velocity = (Vector2){0};
    t->hull_dir = (Vector2){0, 1};
    t->turret_dir = (Vector2){0, 1};
    t->hit_points = TANK_MAX_HP;
    t->hurt_delta = 0;
    t->status_knockback = NULL;
    t->status_recoil = NULL;
    t->weapons = list_create(8);
    list_insert(t->weapons, weapon_machinegun_create((Vector2){0, 8}, 0));
    list_insert(t->weapons, weapon_machinegun_create((Vector2){0, -8}, 180));
    list_insert(t->weapons, weapon_machinegun_create((Vector2){8, 0}, 270));
    list_insert(t->weapons, weapon_machinegun_create((Vector2){-8, 0}, 90));
    return t;
}

void tank_destroy(Tank *t) {
    //TODO: death animation
    //TODO: free weapon state
    list_destroy(t->weapons);
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

void tank_draw(Tank *t, Sprites *ss) {
    // Base: hull and tracks
    RenderTexture2D base_render = LoadRenderTexture(TANK_BASE_PIXELWIDTH, TANK_BASE_PIXELHEIGHT);
    BeginTextureMode(base_render);
        DrawTexture(ss->tank_track, 0, 1, WHITE);
        DrawTexture(ss->tank_track, 13, 1, WHITE);
        DrawTexture(ss->tank_hull, 5, 0, WHITE);
    EndTextureMode();
    Rectangle base_rec_source = {0, 0, base_render.texture.width, base_render.texture.height};
    Rectangle base_rec_dest = {t->rec.x, t->rec.y, TANK_BASE_WIDTH, TANK_BASE_HEIGHT};
    Vector2 base_origin = {TANK_BASE_WIDTH / 2.0f, TANK_BASE_HEIGHT / 2.0f};
    float base_rotation = -RAD2DEG * Vector2Angle(t->hull_dir, (Vector2){0, -1});
    DrawTexturePro(base_render.texture, base_rec_source, base_rec_dest, base_origin, base_rotation, WHITE);
    // Top: Turret and barrel
    TURRET_RT_PIXELSIZE;
    RenderTexture2D top_render = LoadRenderTexture(TURRET_RT_PIXELSIZE, TURRET_RT_PIXELSIZE);
    BeginTextureMode(top_render);
        Texture2D turret = ss->tank_turret;
        Rectangle turret_source = {0, 0, turret.width, turret.height};
        Rectangle turret_dest = {TURRET_RT_MIDDLE, TURRET_RT_MIDDLE, turret.width, turret.height};
        Vector2 turret_origin = {7, 7};
        DrawTexturePro(turret, turret_source, turret_dest, turret_origin, 0, WHITE);
        for (int i = 0; i < list_len(t->weapons); i++) {
            Weapon *w = list_get(t->weapons, i);
            w->draw(w, ss);
        }
    EndTextureMode();
    float top_render_size = TURRET_RT_PIXELSIZE * PIXEL_SIZE;
    Rectangle top_rec_source = {0, 0, top_render.texture.width, top_render.texture.height};
    Vector2 top_pos = _tank_get_turret_pos(t);
    Rectangle top_rec_dest = {top_pos.x, top_pos.y, top_render_size, top_render_size};
    Vector2 top_origin = {top_render_size / 2.0f, top_render_size / 2.0f};
    float top_rotation = -RAD2DEG * Vector2Angle(t->turret_dir, (Vector2){0, -1});
    DrawTexturePro(top_render.texture, top_rec_source, top_rec_dest, top_origin, top_rotation, WHITE);
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
#define BULLET_DAMAGE_BASE 4
#define BULLET_SPREAD (PI / 16.0f)
void _tank_shoot(Tank *t, List *bs, SoundEffects *sfx) {
    float turret_rotation = -RAD2DEG * Vector2Angle((Vector2){0, 1}, t->turret_dir);
    for (int i = 0; i < list_len(t->weapons); i++) {
        Weapon *w = list_get(t->weapons, i);
        w->use(w, bs, _tank_get_turret_pos(t), turret_rotation, sfx);
    }
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

Vector2 _tank_get_turret_pos(Tank *t) {
    return Vector2Subtract((Vector2){t->rec.x, t->rec.y}, Vector2Scale(t->hull_dir, 2 * PIXEL_SIZE));
}
