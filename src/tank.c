#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tank.h"
#include <raymath.h>
#include "util.h"

struct Tank {
    Rectangle hull_rec;
    Vector2 hull_dir;
    Vector2 velocity;
    float turret_radius;
    Vector2 turret_dir;
    int hit_points;
};

Tank *tank_create(int x, int y) {
    Tank *t = malloc(sizeof(Tank));
    check_alloc(t);
    t->hull_rec = (Rectangle){x, y, TANK_HULL_WIDTH, TANK_HULL_HEIGHT};
    t->hull_dir = (Vector2){0, 1};
    t->velocity = (Vector2){0};
    t->turret_radius = TANK_TURRET_RADIUS;
    t->turret_dir = (Vector2){0, 1};
    t->hit_points = TANK_MAX_HP;
    return t;
}

void tank_free(Tank *t) {
    //TODO: explosion
    free(t);
}

#define TANK_MOVE_SPEED 5.0
void tank_move(Tank *t) {
    t->hull_rec.x += t->velocity.x * TANK_MOVE_SPEED;
    t->hull_rec.y += t->velocity.y * TANK_MOVE_SPEED;
}

#define TANK_MOVE_ACC 0.03
#define TANK_MOVE_DEC 0.03
#define TANK_VEL_STAB_RATE 0.2
void tank_velocity_calculate(Tank *t, int dir) {
    int dir_vel;
    if (Vector2Length(t->velocity) != 0){
        float dot = Vector2DotProduct(t->velocity, t->hull_dir);
        if (dot >= 0)
            dir_vel = 1;
        else if (dot < 0)
            dir_vel = -1;
    }
    else
        dir_vel = 0;
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
        t->velocity = Vector2Rotate(t->velocity, TANK_VEL_STAB_RATE*diff);
}

#define TANK_HULL_ROT_SPEED 0.08
#define TANK_HULL_ROT_ACC 0.08
#define TANK_HULL_ROT_DEC 0.16
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

#define TANK_TURRET_ROT_SPEED 0.1
#define TANK_TURRET_ROT_ACC 0.08
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

#define TANK_SHOT_DELAY 60
void tank_shoot(Tank *t, List *bs) {
    // TODO: recoil
    static int shot_delta = 0;
    if (shot_delta >= TANK_SHOT_DELAY) {
        Vector2 turret_pos = Vector2Subtract((Vector2){t->hull_rec.x, t->hull_rec.y}, Vector2Scale(t->hull_dir, t->hull_rec.height / 2 - t->hull_rec.width / 2));
        Vector2 barrel_end = Vector2Add(turret_pos, Vector2Scale(t->turret_dir, t->turret_radius + TANK_BARREL_LENGTH - TANK_TURRET_RADIUS * 0.05 - BULLET_RADIUS / 2));
        Bullet *b = bullet_create(barrel_end, t->turret_dir);
        list_insert(bs, b);
        shot_delta = 0;
    }
    else
        shot_delta++;
}

void tank_reduce_hp(Tank *t, int hp) {
    t->hit_points -= hp;
    if (t->hit_points < 0)
        t->hit_points = 0;
}

#define TANK_HULL_COLOR DARKGREEN
#define TANK_TRACK_COLOR DARKGRAY
#define TANK_TURRET_COLOR GREEN
void tank_draw(Tank *t) {
    Vector2 hull_center = {t->hull_rec.width / 2, t->hull_rec.height / 2};
    float hull_rotation = -RAD2DEG * Vector2Angle(t->hull_dir, (Vector2){0, -1});
    DrawRectanglePro(t->hull_rec, hull_center, hull_rotation, TANK_HULL_COLOR);
    Rectangle track_rec = {t->hull_rec.x, t->hull_rec.y, TANK_TRACK_WIDTH, t->hull_rec.height};
    DrawRectanglePro(track_rec, hull_center, hull_rotation, TANK_TRACK_COLOR);
    DrawRectanglePro(track_rec, hull_center, 180 + hull_rotation, TANK_TRACK_COLOR);
    Vector2 turret_pos = Vector2Subtract((Vector2){t->hull_rec.x, t->hull_rec.y}, Vector2Scale(t->hull_dir, t->hull_rec.height / 2 - t->hull_rec.width / 2));
    DrawCircleV(turret_pos, t->turret_radius, TANK_TURRET_COLOR);
    Vector2 barrel_pos = Vector2Add(turret_pos, Vector2Scale(t->turret_dir, t->turret_radius + TANK_BARREL_LENGTH / 2 - TANK_TURRET_RADIUS * 0.05));
    Rectangle barrel_rec = {barrel_pos.x, barrel_pos.y, TANK_BARREL_WIDTH, TANK_BARREL_LENGTH};
    Vector2 barrel_center = {TANK_BARREL_WIDTH / 2, TANK_BARREL_LENGTH / 2};
    float turret_rotation = -RAD2DEG * Vector2Angle(t->turret_dir, (Vector2){0, -1});
    DrawRectanglePro(barrel_rec, barrel_center, turret_rotation, TANK_TURRET_COLOR);
    Vector2 tp = (Vector2){t->hull_rec.x, t->hull_rec.y};
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
    return (Vector2){t->hull_rec.x, t->hull_rec.y};
}
