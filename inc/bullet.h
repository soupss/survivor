#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>
#include "sprites.h"

typedef struct Bullet Bullet;
Bullet* bullet_create(Vector2 pos, Vector2 dir);
void bullet_destroy(Bullet *b);
void bullet_update(Bullet *b);
void bullet_draw(Bullet *b, Sprites *ss);
int bullet_get_damage(Bullet *b);
bool bullet_is_out_of_bounds(Bullet *b);
Vector2 bullet_get_pos(Bullet *b);
Vector2 bullet_get_dir(Bullet *b);

#endif
