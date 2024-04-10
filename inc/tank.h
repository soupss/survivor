#ifndef TANK_H
#define TANK_H

#include <raylib.h>
#include "bullet.h"
#include "list.h"

typedef struct Tank Tank;
Tank *tank_create(Vector2 pos);
void tank_free(Tank *t);
void tank_set_dir_target(Tank *t, Vector2 dir);
void tank_move(Tank *t);
void tank_shoot(Tank *t, List *bs);
void tank_draw(Tank *t);
Vector2 tank_get_pos(Tank *t);

#endif
