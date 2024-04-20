#ifndef MOB_H
#define MOB_H

#include <raylib.h>
#include "list.h"

typedef struct Mob Mob;
Mob *mob_create(Vector2 pos);
void mob_free(Mob *m);
void mob_update(Mob *m, Vector2 target, List *ms);
void mob_draw(Mob *m);
int mob_attack(Mob *m);
Vector2 mob_get_pos(Mob *m);
bool mob_is_dead(Mob *m);
void mob_reduce_hp(Mob *m, int hp);
void mob_apply_knockback(Mob *m, float distance, float angle, float duration);

typedef struct ExpOrb ExpOrb;
ExpOrb *exp_create(Vector2 pos);
void exp_free(ExpOrb *xp);
void exp_update(ExpOrb *xp, Vector2 target);
void exp_draw(ExpOrb *xp);
Vector2 exp_get_pos(ExpOrb *xp);
float exp_get_points(ExpOrb *xp);
#endif
