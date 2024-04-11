#ifndef UTIL_H
#define UTIL_H

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

#endif
