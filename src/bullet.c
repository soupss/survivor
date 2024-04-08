#include <stdlib.h>
#include <stdio.h>
#include "bullet.h"
#include <raymath.h>
#include "runtime_constants.h"

#define BULLET_COLOR DARKGRAY

struct Bullet {
    Vector2 pos;
    int radius;
    Vector2 dir;
};

Bullet* bullet_create(Vector2 pos, Vector2 dir) {
    Bullet *b = malloc(sizeof(Bullet));
    if (b == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    b->pos = pos;
    b->radius = BULLET_RADIUS;
    b->dir = dir;
    return b;
}

void bullet_destroy(Bullet *b) {
    free(b);
}

void bullet_update(Bullet *b) {
    b->pos = Vector2Add(b->pos, Vector2Scale(b->dir, 3));
    //TODO: free bullets that are out of screen
}

void bullet_draw(Bullet *b) {
    DrawCircleV(b->pos, b->radius, BULLET_COLOR);
}
