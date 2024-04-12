#ifndef TANK_H
#define TANK_H

#include <raylib.h>
#include "bullet.h"
#include "list.h"

typedef struct Tank Tank;
Tank *tank_create(int x, int y);
void tank_free(Tank *t);
void tank_set_move_dir(Tank *t, int dir);
void tank_hull_set_rot_dir(Tank *t, int dir);
void tank_turret_set_rot_dir(Tank *t, int dir);
void tank_shoot(Tank *t, List *bs);
void tank_draw(Tank *t);
Vector2 tank_get_pos(Tank *t);

#endif
