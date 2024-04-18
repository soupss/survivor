#ifndef MOB_H
#define MOB_H

#include "raylib.h"

typedef struct Mob Mob;
Mob *mob_create(Vector2 pos);
void mob_free(Mob *m);
void mob_update(Mob *m, Vector2 target);
void mob_draw(Mob *m);
Vector2 mob_get_pos(Mob *m);
bool mob_is_dead(Mob *m);
void mob_reduce_hp(Mob *m, int hp);
void mob_apply_knockback(Mob *m, float distance, float angle);

typedef struct Exp Exp;
Exp *exp_create(Vector2 pos);
void exp_free(Exp *xp);
void exp_update(Exp *xp, Vector2 target);
void exp_draw(Exp *xp);
Vector2 exp_get_pos(Exp *xp);
float exp_get_points(Exp *xp);
#endif
