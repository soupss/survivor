#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bullet.h"
#include <raymath.h>
#include "util.h"

struct Bullet {
    Vector2 pos;
    Vector2 dir;
    int distance_traveled;
};

Bullet* bullet_create(Vector2 pos, Vector2 dir) {
    Bullet *b = malloc(sizeof(Bullet));
    util_check_alloc(b);
    b->pos = pos;
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
void bullet_draw(Bullet *b, Sprites *ss) {
    Vector2 size = {BULLET_PIXELWIDTH * PIXEL_SIZE, BULLET_PIXELHEIGHT * PIXEL_SIZE};
    Rectangle rec_source = {0, 0, BULLET_PIXELWIDTH, BULLET_PIXELHEIGHT};
    Rectangle rec_dest = {b->pos.x, b->pos.y, size.x, size.y};
    Vector2 origin = {size.x / 2, size.y / 2};
    float rotation = -RAD2DEG * Vector2Angle(b->dir, (Vector2){0, -1});
    DrawTexturePro(ss->bullet, rec_source, rec_dest, origin, rotation, WHITE);
}

#define BULLET_FULLDMG_RANGE SCREEN_HEIGHT * 0.2
#define BULLET_DAMAGE_DROPOFF_FACTOR 0.025
int bullet_get_damage(Bullet *b) {
    if (b->distance_traveled >= BULLET_FULLDMG_RANGE) {
        double d = b->distance_traveled - BULLET_FULLDMG_RANGE;
        return BULLET_DAMAGE - round(BULLET_DAMAGE_DROPOFF_FACTOR * d);
    }
    else
        return BULLET_DAMAGE;
}

bool bullet_is_out_of_bounds(Bullet *b) {
    float size = BULLET_PIXELWIDTH * PIXEL_SIZE;
    if (b->pos.x - size < 0 ||
        b->pos.x + size > SCREEN_WIDTH ||
        b->pos.y - size < 0 ||
        b->pos.y + size > SCREEN_HEIGHT)
        return true;
    else
        return false;
}

Vector2 bullet_get_pos(Bullet *b) {
    return b->pos;
}

Vector2 bullet_get_dir(Bullet *b) {
    return b->dir;
}
