#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bullet.h"
#include <raymath.h>
#include "util.h"

struct Bullet {
    Vector2 pos;
    float radius;
    Vector2 dir;
    int distance_traveled;
};

Bullet* bullet_create(Vector2 pos, Vector2 dir) {
    Bullet *b = malloc(sizeof(Bullet));
    util_check_alloc(b);
    b->pos = pos;
    b->radius = BULLET_RADIUS;
    b->dir = dir;
    b->distance_traveled = 0;
    return b;
}

void bullet_destroy(Bullet *b) {
    free(b);
}

#define BULLET_SPEED 15
void bullet_update(Bullet *b) {
    b->pos = Vector2Add(b->pos, Vector2Scale(b->dir, BULLET_SPEED));
    b->distance_traveled += BULLET_SPEED;
}

#define BULLET_COLOR DARKGRAY
void bullet_draw(Bullet *b) {
    DrawCircleV(b->pos, b->radius, BULLET_COLOR);
}

#define BULLET_DAMAGE 4
#define BULLET_FULLDMG_RANGE SCREEN_HEIGHT * 0.8
#define BULLET_DAMAGE_DROPOFF_FACTOR 0.002
int bullet_get_damage(Bullet *b) {
    if (b->distance_traveled >= BULLET_FULLDMG_RANGE) {
        double d = b->distance_traveled - BULLET_FULLDMG_RANGE;
        return BULLET_DAMAGE - round(BULLET_DAMAGE_DROPOFF_FACTOR * d);
    }
    else
        return BULLET_DAMAGE;
}

bool bullet_out_of_bounds(Bullet *b) {
    if (b->pos.x < 0 - b->radius ||
        b->pos.x > SCREEN_WIDTH + b->radius ||
        b->pos.y < 0 - b->radius ||
        b->pos.y > SCREEN_HEIGHT + b->radius) return true;
    else
        return false;
}

Vector2 bullet_get_pos(Bullet *b) {
    return b->pos;
}

Vector2 bullet_get_dir(Bullet *b) {
    return b->dir;
}
