#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>

typedef struct Bullet Bullet;
Bullet* bullet_create(Vector2 pos, Vector2 dir);
void bullet_destroy(Bullet *b);
void bullet_update(Bullet *b);
void bullet_draw(Bullet *b);
bool bullet_out_of_bounds(Bullet *b);

#endif
