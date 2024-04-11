#ifndef MOB_H
#define MOB_H

#include "raylib.h"

typedef struct Mob Mob;
Mob *mob_create(Vector2 pos);
void mob_free(Mob *m);
void mob_update(Mob *m, Vector2 target);
void mob_draw(Mob *m);
Vector2 mob_get_pos(Mob *m);

#endif
