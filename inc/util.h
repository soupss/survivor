#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern float TANK_HULL_WIDTH;
extern float TANK_HULL_HEIGHT;
extern float TANK_TRACK_WIDTH;
extern float TANK_TURRET_RADIUS;
extern float TANK_BARREL_WIDTH;
extern float TANK_BARREL_LENGTH;
extern float BULLET_RADIUS;
extern float MOB_RADIUS;
void init_constants(int screen_width, int screen_height);
void check_alloc(void *p);
Vector2 Vector2AddMagnitude(Vector2 v, float add);
Vector2 Vector2SubtractMagnitude(Vector2 v, float subtract);

#endif
