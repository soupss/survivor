#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>
#include <list.h>

#define BACKGROUND_COLOR RAYWHITE
#define EXP_COLOR SKYBLUE
#define TANK_MAX_HP 20

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern float TANK_HULL_WIDTH;
extern float TANK_HULL_HEIGHT;
extern float TANK_TRACK_WIDTH;
extern float TANK_TURRET_RADIUS;
extern float TANK_BARREL_WIDTH;
extern float TANK_BARREL_LENGTH;
extern float TANK_HITBOX_RADIUS;
extern float BULLET_RADIUS;
extern float MOB_RADIUS;
extern float EXPORB_RADIUS;
extern float EXPORB_PICKUPRANGE;
extern float EXPBAR_WIDTH;
extern float EXPBAR_HEIGHT;
void init_constants(int screen_width, int screen_height);
void check_alloc(void *p);
Vector2 Vector2AddMagnitude(Vector2 v, float add);
Vector2 Vector2SubtractMagnitude(Vector2 v, float subtract);
// TODO: decide
Vector2 calculate_separation_list(void *p, List *l);
Vector2 calculate_separation_entity(void *p1, void *p2);

#endif
