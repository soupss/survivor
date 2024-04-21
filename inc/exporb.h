#ifndef EXP_ORB
#define EXP_ORB

#include <raylib.h>

typedef struct ExpOrb ExpOrb;
ExpOrb *exp_create(Vector2 pos);
void exp_free(ExpOrb *xp);
void exp_update(ExpOrb *xp, Vector2 target);
void exp_draw(ExpOrb *xp);
Vector2 exp_get_pos(ExpOrb *xp);
float exp_get_points(ExpOrb *xp);


#endif
