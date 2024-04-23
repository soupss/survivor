#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <raymath.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
float TANK_HULL_WIDTH;
float TANK_HULL_HEIGHT;
float TANK_TRACK_WIDTH;
float TANK_TURRET_RADIUS;
float TANK_BARREL_WIDTH;
float TANK_BARREL_LENGTH;
float TANK_HITBOX_RADIUS;
float BULLET_RADIUS;
float MOB_RADIUS;
float EXPORB_RADIUS;
float EXPORB_PICKUPRANGE;
float EXPBAR_WIDTH;
float EXPBAR_HEIGHT;

void init_constants(int screen_width, int screen_height) {
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
    TANK_HULL_WIDTH = screen_width / 40.0;
    TANK_HULL_HEIGHT = TANK_HULL_WIDTH * 1.23;
    TANK_TRACK_WIDTH = TANK_HULL_WIDTH * 0.25;
    TANK_TURRET_RADIUS = TANK_HULL_WIDTH * 0.3;
    BULLET_RADIUS = TANK_TURRET_RADIUS / 4.0;
    TANK_BARREL_WIDTH = BULLET_RADIUS * 1.8;
    TANK_BARREL_LENGTH = TANK_TURRET_RADIUS;
    TANK_HITBOX_RADIUS = TANK_HULL_WIDTH / 2;
    MOB_RADIUS = screen_width / 90.0;
    EXPORB_RADIUS = TANK_TURRET_RADIUS / 2.0;
    EXPORB_PICKUPRANGE = TANK_HULL_HEIGHT * 1.5;
    EXPBAR_WIDTH = screen_width;
    EXPBAR_HEIGHT = screen_height / 64.0;
}

void check_alloc(void *p) {
    if (p == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
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
