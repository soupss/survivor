#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int TANK_RADIUS;
int MOB_RADIUS;
int BULLET_RADIUS;
int BARREL_WIDTH;
int BARREL_LENGTH;

void init_constants(int screen_width, int screen_height) {
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
    TANK_RADIUS = screen_width / 90;
    MOB_RADIUS = screen_width / 70;
    BULLET_RADIUS = TANK_RADIUS / 4;
    BARREL_WIDTH = BULLET_RADIUS * 2;
    BARREL_LENGTH = TANK_RADIUS;
    printf("runtime constants:\nSCREEN_WIDTH:%d\nSCREEN_HEIGHT:%d\nTANK_RADIUS:%d\nMOB_RADIUS:%d\nBULLET_RADIUS:%d\nBARREL_WIDTH:%d\nBARREL_LENGTH:%d\n", SCREEN_WIDTH, SCREEN_HEIGHT, TANK_RADIUS, MOB_RADIUS, BULLET_RADIUS, BARREL_WIDTH, BARREL_LENGTH);
}

void check_alloc(void *p) {
    if (p == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
}
