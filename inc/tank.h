#ifndef TANK_H
#define TANK_H

#include <raylib.h>
#include "bullet.h"
#include "list.h"

typedef struct Tank Tank;
Tank *tank_create(int x, int y);
void tank_destroy(Tank *t);
void tank_update(Tank *t, List *bs, List *ms);
void tank_draw(Tank *t);
void tank_velocity_calculate(Tank *t, int dir);
void tank_hull_rotate(Tank *t, int dir);
void tank_turret_rotate(Tank *t, int dir);
void tank_hp_reduce(Tank *t, int hp);
void tank_set_status_knockback(Tank *t, float distance, float angle, int duration);
bool tank_is_dead(Tank *t);
int tank_get_hp(Tank *t);
Vector2 tank_get_pos(Tank *t);

#endif
