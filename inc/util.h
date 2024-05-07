#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>
#include <list.h>
#include "tank.h"

#define TANK_MAX_HP 20
#define BULLET_DAMAGE 4
#define TURRET_RT_PIXELSIZE 100
#define TURRET_RT_MIDDLE 50
#define BACKGROUND_COLOR RAYWHITE
#define EXP_COLOR SKYBLUE

extern float PIXEL_SIZE;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern float TANK_BASE_WIDTH;
extern float TANK_BASE_HEIGHT;
extern float TANK_HITBOX_RADIUS;
extern float MOB_HITBOX_RADIUS;

extern float EXPORB_RADIUS;
extern float EXPORB_PICKUPRANGE;
extern float EXPBAR_WIDTH;
extern float EXPBAR_HEIGHT;
void util_init_constants(int screen_width, int screen_height);
void util_check_alloc(void *p);
Vector2 util_separation_from_mobs_v(Vector2 pos, float radius, List *ms, float separation_factor);
Vector2 util_separation_from_mobs(int x, int y, float radius, List *ms, float separation_factor);
Vector2 Vector2AddMagnitude(Vector2 v, float add);
Vector2 Vector2SubtractMagnitude(Vector2 v, float subtract);

#endif
