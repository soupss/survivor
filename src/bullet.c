#include <stdlib.h>
#include <stdio.h>
#include "bullet.h"
#include <raymath.h>
#include "util.h"

#define BULLET_COLOR DARKGRAY
#define BULLET_SPEED 15

struct Bullet {
    Vector2 pos;
    int radius;
    Vector2 dir;
};

Bullet* bullet_create(Vector2 pos, Vector2 dir) {
    Bullet *b = malloc(sizeof(Bullet));
    check_alloc(b);
    b->pos = pos;
    b->radius = BULLET_RADIUS;
    b->dir = dir;
    return b;
}

void bullet_free(Bullet *b) {
    free(b);
}

void bullet_update(Bullet *b) {
    b->pos = Vector2Add(b->pos, Vector2Scale(b->dir, BULLET_SPEED));
}

void bullet_draw(Bullet *b) {
    DrawCircleV(b->pos, b->radius, BULLET_COLOR);
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
