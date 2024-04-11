#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tank.h"
#include <raymath.h>
#include "util.h"

#define TANK_MOVE_SPEED 2.0
#define TANK_SHOT_DELAY 60
#define TANK_HULL_ROTATION_SPEED 0.08
#define TANK_TURRET_ROTATION_SPEED 0.1
#define TANK_TURRET_WINDUP_SPEED 0.08
#define TANK_TURRET_WINDDOWN_SPEED 0.12
//TODO: prettier colors (not just tank colors)
#define TANK_HULL_COLOR DARKGREEN
#define TANK_TRACK_COLOR DARKGRAY
#define TANK_TURRET_COLOR GREEN

struct Tank {
    Rectangle hull_rec;
    Vector2 hull_dir;
    float turret_radius;
    Vector2 turret_dir;
};

Tank *tank_create(int x, int y) {
    Tank *t = malloc(sizeof(Tank));
    check_alloc(t);
    t->hull_rec = (Rectangle){x, y, TANK_HULL_WIDTH, TANK_HULL_HEIGHT};
    t->hull_dir = (Vector2){0, 1};
    t->turret_radius = TANK_TURRET_RADIUS;
    t->turret_dir = (Vector2){0, 1};
    return t;
}

void tank_free(Tank *t) {
    //TODO: explosion
    free(t);
}

void tank_move(Tank *t, int dir) {
    //TODO: smooth movement
    t->hull_rec.x += dir * TANK_MOVE_SPEED * t->hull_dir.x;
    t->hull_rec.y += dir * TANK_MOVE_SPEED * t->hull_dir.y;
}

void tank_hull_rotate(Tank *t, int dir) {
    t->hull_dir = Vector2Rotate(t->hull_dir, dir * TANK_HULL_ROTATION_SPEED);
}

void tank_turret_rotate(Tank *t, int dir) {
    static int dir_curr = 0;
    static float windup = 0;
    if (dir != 0 && dir != dir_curr) {
        windup = 0;
        dir_curr = dir;
    }
    if (dir != 0 && windup < 1.0) {
        windup += TANK_TURRET_WINDUP_SPEED;
        if (windup > 1.0) {
            windup = 1.0;
        }
    }
    else if (dir == 0 && windup > 0.0) {
        windup -= TANK_TURRET_WINDDOWN_SPEED;
        if (windup < 0) {
            windup = 0;
            dir_curr = 0;
        }
    }
    if (windup > 0.0) {
        t->turret_dir = Vector2Rotate(t->turret_dir, windup * dir_curr * TANK_TURRET_ROTATION_SPEED);
    }
}

void tank_shoot(Tank *t, List *bs) {
    static int shot_delta = 0;
    if (shot_delta >= TANK_SHOT_DELAY) {
        Vector2 turret_pos = Vector2Subtract((Vector2){t->hull_rec.x, t->hull_rec.y}, Vector2Scale(t->hull_dir, t->hull_rec.height / 2 - t->hull_rec.width / 2));
        Vector2 barrel_end = Vector2Add(turret_pos, Vector2Scale(t->turret_dir, t->turret_radius + TANK_BARREL_LENGTH - TANK_TURRET_RADIUS * 0.05 - BULLET_RADIUS / 2));
        Bullet *b = bullet_create(barrel_end, t->turret_dir);
        list_insert(bs, b);
        shot_delta = 0;
    }
    else shot_delta++;
}

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
}

Vector2 tank_get_pos(Tank *t) {
    return (Vector2){t->hull_rec.x, t->hull_rec.y};
}
