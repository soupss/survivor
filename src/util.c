#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
float TANK_HULL_WIDTH;
float TANK_HULL_HEIGHT;
float TANK_TRACK_WIDTH;
float TANK_TURRET_RADIUS;
float TANK_BARREL_WIDTH;
float TANK_BARREL_LENGTH;
float BULLET_RADIUS;
float MOB_RADIUS;

void init_constants(int screen_width, int screen_height) {
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
    TANK_HULL_WIDTH = screen_width / 30.0;
    TANK_HULL_HEIGHT = TANK_HULL_WIDTH * 1.23;
    TANK_TRACK_WIDTH = TANK_HULL_WIDTH * 0.25;
    TANK_TURRET_RADIUS = TANK_HULL_WIDTH * 0.3;
    BULLET_RADIUS = TANK_TURRET_RADIUS / 4.0;
    TANK_BARREL_WIDTH = BULLET_RADIUS * 1.8;
    TANK_BARREL_LENGTH = TANK_TURRET_RADIUS;
    MOB_RADIUS = screen_width / 70.0;
}

void check_alloc(void *p) {
    if (p == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
}
