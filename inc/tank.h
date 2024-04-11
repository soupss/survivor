#ifndef TANK_H
#define TANK_H

#include <raylib.h>
#include "bullet.h"
#include "list.h"

typedef struct Tank Tank;
Tank *tank_create(int x, int y);
void tank_free(Tank *t);
void tank_move(Tank *t, int dir);
void tank_rotate(Tank *t, int dir);
void tank_turret_rotate(Tank *t, int dir);
void tank_draw(Tank *t);
Vector2 tank_get_pos(Tank *t);

#endif
