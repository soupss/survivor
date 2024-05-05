#ifndef MOB_H
#define MOB_H

#include <raylib.h>
#include "list.h"
#include "tank.h"

typedef struct Mob Mob;
Mob *mob_create(Vector2 pos);
void mob_destroy(Mob *m);
void mob_update(Mob *m, Vector2 target, List *ms);
void mob_draw(Mob *m, Sprites *ss);
int mob_attack(Mob *m);
void mob_hp_reduce(Mob *m, int hp);
void mob_set_status_knockback(Mob *m, float distance, float angle, int duration);
bool mob_is_dead(Mob *m);
Vector2 mob_get_pos(Mob *m);
Vector2 mob_get_dir(Mob *m);

#endif
