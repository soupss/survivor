#ifndef UTIL_H
#define UTIL_H

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int PLAYER_RADIUS;
extern int MOB_RADIUS;
extern int BULLET_RADIUS;
extern int BARREL_WIDTH;
extern int BARREL_LENGTH;
void init_constants(int screen_width, int screen_height);
void check_alloc(void *p);

#endif
