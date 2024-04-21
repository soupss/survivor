#ifndef EXP_ORB
#define EXP_ORB

#include <raylib.h>

typedef struct ExpOrb ExpOrb;
ExpOrb *exporb_create(Vector2 pos);
void exporb_destroy(ExpOrb *xp);
void exporb_update(ExpOrb *xp, Vector2 target);
void exporb_draw(ExpOrb *xp);
Vector2 exporb_get_pos(ExpOrb *xp);
float exporb_get_points(ExpOrb *xp);


#endif
