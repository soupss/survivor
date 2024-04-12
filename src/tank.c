#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tank.h"
#include <raymath.h>
#include "util.h"

#define TANK_MOVE_SPEED 5.0
#define TANK_MOVE_ACC 0.03
#define TANK_MOVE_DEC 0.03
#define TANK_SHOT_DELAY 60
#define TANK_HULL_ROT_SPEED 0.08
#define TANK_HULL_ROT_ACC 0.08
#define TANK_HULL_ROT_DEC 0.16
#define TANK_TURRET_ROT_SPEED 0.1
#define TANK_TURRET_ROT_ACC 0.08
#define TANK_TURRET_ROT_DEC 0.12
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

void tank_set_move_dir(Tank *t, int dir) {
    static int dir_curr = 0;
    static float momentum = 0.0;
    static Vector2 hull_dir_last = {0.0};
    if (dir != 0) hull_dir_last = t->hull_dir;
    // change direction
    if (dir != dir_curr && momentum > 0.0) {
        if (dir != 0) momentum -= (TANK_MOVE_DEC + TANK_MOVE_ACC);
        else momentum -= TANK_MOVE_DEC;
        if (momentum < 0.0) {
            momentum = 0.0;
            dir_curr = dir;
        }
    }
    else {
        // accelerate
        if (dir != 0 && momentum < 1.0) {
            momentum += TANK_MOVE_ACC;
            if (momentum > 1.0) momentum = 1.0;
        }
        // stop
        else if (dir == 0 && momentum > 0.0) {
            momentum -= TANK_MOVE_DEC;
            if (momentum < 0.0) {
                momentum = 0.0;
                dir_curr = 0;
            }
        }
    }
    if (momentum > 0) {
        t->hull_rec.x += momentum * dir_curr * TANK_MOVE_SPEED * hull_dir_last.x;
        t->hull_rec.y += momentum * dir_curr * TANK_MOVE_SPEED * hull_dir_last.y;
    }
}

void tank_hull_set_rot_dir(Tank *t, int dir) {
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
        else dir_curr = dir;
    }
    else {
        if (dir != 0 && momentum < 1.0) {
            momentum += TANK_HULL_ROT_ACC;
            if (momentum > 1.0) {
                momentum = 1.0;
            }
        }
        else if (dir == 0 && momentum > 0.0) {
            momentum -= TANK_HULL_ROT_DEC;
            if (momentum < 0) {
                momentum = 0;
                dir_curr = 0;
            }
        }
    }
    if (momentum > 0.0) {
        t->hull_dir = Vector2Rotate(t->hull_dir, momentum * dir_curr * TANK_HULL_ROT_SPEED);
    }
}

void tank_turret_set_rot_dir(Tank *t, int dir) {
    static int dir_curr = 0;
    static float momentum = 0;
    if (dir != 0 && dir != dir_curr) {
        momentum = 0;
        dir_curr = dir;
    }
    if (dir != 0 && momentum < 1.0) {
        momentum += TANK_TURRET_ROT_ACC;
        if (momentum > 1.0) {
            momentum = 1.0;
        }
    }
    else if (dir == 0 && momentum > 0.0) {
        momentum -= TANK_TURRET_ROT_DEC;
        if (momentum < 0) {
            momentum = 0;
            dir_curr = 0;
        }
    }
    if (momentum > 0.0) {
        t->turret_dir = Vector2Rotate(t->turret_dir, momentum * dir_curr * TANK_TURRET_ROT_SPEED);
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
