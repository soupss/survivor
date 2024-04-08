#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "bullet.h"
#include "list.h"

typedef struct Player Player;
Player *player_create(Vector2 pos);
void player_destroy(Player *p);
void player_set_dir_target(Player *p, Vector2 dir);
void player_move(Player *p);
void player_shoot(Player *p, List *bs);
void player_draw(Player *p);

#endif
