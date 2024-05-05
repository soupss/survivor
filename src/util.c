#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <raymath.h>
#include "tank.h"
#include "mob.h"
#include "sprites.h"

float PIXEL_SIZE;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
float TANK_BASE_WIDTH;
float TANK_BASE_HEIGHT;
float TANK_HITBOX_RADIUS;
float MOB_HITBOX_RADIUS;
float EXPORB_RADIUS;
float EXPORB_PICKUPRANGE;
float EXPBAR_WIDTH;
float EXPBAR_HEIGHT;

void util_init_constants(int screen_width, int screen_height) {
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
    PIXEL_SIZE = screen_width / 400.0f;
    TANK_BASE_WIDTH = 16 * PIXEL_SIZE;
    TANK_BASE_HEIGHT = 16 * PIXEL_SIZE;
    TANK_HITBOX_RADIUS = TANK_BASE_WIDTH / 2;
    MOB_HITBOX_RADIUS = (SPIDER_PIXELHEIGHT * PIXEL_SIZE) / 2;
    EXPORB_RADIUS = EXPORB_PIXELWIDTH * PIXEL_SIZE;
    EXPORB_PICKUPRANGE = TANK_BASE_HEIGHT * 1;
    EXPBAR_WIDTH = screen_width;
    EXPBAR_HEIGHT = screen_height / 64.0;
}

void util_check_alloc(void *p) {
    if (p == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
}

Vector2 util_separation_from_mobs_v(Vector2 pos, float radius, List *ms, float separation_factor) {
    Vector2 steer = {0};
    int count = 0;
    for (int i = 0; i < list_len(ms); i++) {
        Mob *other = list_get(ms, i);
        Vector2 pos_other = mob_get_pos(other);
        if (Vector2Equals(pos, pos_other))
            continue;
        Vector2 diff = Vector2Subtract(pos, pos_other);
        float d = Vector2Length(diff);
        if (d < radius + MOB_HITBOX_RADIUS) {
            float separation = (separation_factor * radius) / d;
            diff = Vector2Scale(Vector2Normalize(diff), separation);
            steer = Vector2Add(steer, diff);
            count++;
        }
    }
    if (count > 0) {
        steer = Vector2Scale(steer, 1 / (float)count);
    }
    return steer;
}

Vector2 util_separation_from_mobs(int x, int y, float radius, List *ms, float separation_factor) {
    return util_separation_from_mobs_v((Vector2){x, y}, radius, ms, separation_factor);
}

Vector2 Vector2AddMagnitude(Vector2 v, float add) {
    float len = Vector2Length(v);
    float len_new = len + add;
    if (len == 0) {
        printf("Vector2AddMagnitude: Unable to determine rotation of vector");
        return (Vector2){0, -add};
    }
    return Vector2Scale(Vector2Normalize(v), len_new);
}

Vector2 Vector2SubtractMagnitude(Vector2 v, float subtract) {
    float len = Vector2Length(v);
    float len_new = len - subtract;
    if (len == 0 || len_new < 0)
        return Vector2Zero();
    return Vector2Scale(Vector2Normalize(v), len_new);
}
