#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tank.h"
#include <raymath.h>
#include "util.h"

#define TANK_TURN_SPEED 0.12
#define TANK_COLOR GREEN
#define TANK_SHOT_DELAY 60
#define TANK_SPEED_INITIAL 2.0

struct Tank {
    Vector2 pos;
    int radius;
    Vector2 dir;
    float speed;
    Vector2 dir_target;
    int shot_delta;
};

Tank *tank_create(Vector2 pos) {
    Tank *t = malloc(sizeof(Tank));
    check_alloc(t);
    t->pos = pos;
    t->radius = TANK_RADIUS;
    t->dir = (Vector2){0, -1};
    t->speed = TANK_SPEED_INITIAL;
    t->shot_delta = 0;
    return t;
}

void tank_free(Tank *t) {
    free(t);
}

void tank_set_dir_target(Tank *t, Vector2 dir) {
    t->dir_target = dir;
}

void tank_move(Tank *t) {
    if (Vector2Length(t->dir_target) > 0) {
        Vector2 normal = {t->dir.y, -t->dir.x};
        float angle = TANK_TURN_SPEED;
        float d = Vector2DotProduct(normal, t->dir_target);
        if (d > 0) angle *= -1;
        float angle_to_target = Vector2Angle(t->dir, t->dir_target);
        if (fabs(angle_to_target) < fabs(angle)) {
            t->dir = Vector2Rotate(t->dir, angle_to_target);
        } else {
            t->dir = Vector2Rotate(t->dir, angle);
        }
        t->pos = Vector2Add(t->pos, Vector2Scale(t->dir, t->speed));
    }
}

void tank_shoot(Tank *t, List *bs) {
    if (t->shot_delta >= TANK_SHOT_DELAY) {
        Vector2 bullet_start = Vector2Add(t->pos, Vector2Scale(t->dir, t->radius + BARREL_LENGTH - BULLET_RADIUS * 2 - (float)BARREL_LENGTH/20));
        Bullet *b = bullet_create(bullet_start, t->dir);
        list_insert(bs, b);
        t->shot_delta = 0;
    }
    else t->shot_delta++;
}

void tank_draw(Tank *t) {
    DrawCircleV(t->pos, t->radius, TANK_COLOR);
    Vector2 barrel_pos = Vector2Add(t->pos, Vector2Scale(t->dir, t->radius + BARREL_LENGTH/2 - (float)BARREL_LENGTH/20));
    float barrel_rotation = -RAD2DEG * Vector2Angle(t->dir, (Vector2){0, -1});
    Rectangle r = {barrel_pos.x, barrel_pos.y, BARREL_WIDTH, BARREL_LENGTH};
    Vector2 v = {BARREL_WIDTH/2, BARREL_LENGTH/2};
    DrawRectanglePro(r, v, barrel_rotation, TANK_COLOR);
}

Vector2 tank_get_pos(Tank *t) {
    return t->pos;
}
