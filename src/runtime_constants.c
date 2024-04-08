#include <stdio.h>
#include "runtime_constants.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;
int PLAYER_RADIUS;
int BULLET_RADIUS;
int BARREL_WIDTH;
int BARREL_LENGTH;

void init_constants(int screen_width, int screen_height) {
    SCREEN_WIDTH = screen_width;
    SCREEN_HEIGHT = screen_height;
    PLAYER_RADIUS = screen_width / 90;
    BULLET_RADIUS = PLAYER_RADIUS / 4;
    BARREL_WIDTH = BULLET_RADIUS * 2;
    BARREL_LENGTH = PLAYER_RADIUS;

    printf("runtime constants:\nSCREEN_WIDTH:%d\nSCREEN_HEIGHT:%d\nPLAYER_RADIUS:%d\nBULLET_RADIUS:%d\nBARREL_WIDTH:%d\nBARREL_LENGTH:%d\n", SCREEN_WIDTH, SCREEN_HEIGHT, PLAYER_RADIUS, BULLET_RADIUS, BARREL_WIDTH, BARREL_LENGTH);
}
