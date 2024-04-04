#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct Player Player;
Player* player_create(Vector2 pos);
void player_update(Player *p);
void player_draw(Player *p);
void player_delete(Player *p);

#endif
