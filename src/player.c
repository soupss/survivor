#include <stdlib.h>
#include "player.h"

#define PLAYER_SPEED 5

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Dir;

struct Player {
    Vector2 pos;
    Vector2 size;
    Dir dir;
};

Player* player_create(Vector2 pos, Vector2 size) {
    Player *p = malloc(sizeof(Player));
    if (p) {
        p->pos = pos;
        p->size = size;
        p->dir = DOWN;
    }
    return p;
}

void player_update(Player *p) {
    if (IsKeyDown(KEY_UP)) {
        p->pos.y -= PLAYER_SPEED;
        p->dir = UP;
    }
    if (IsKeyDown(KEY_DOWN)) {
        p->pos.y += PLAYER_SPEED;
        p->dir = DOWN;
    }
    if (IsKeyDown(KEY_LEFT)) {
        p->pos.x -= PLAYER_SPEED;
        p->dir = LEFT;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        p->pos.x += PLAYER_SPEED;
        p->dir = RIGHT;
    }
}

void player_draw(Player *p) {
    Color color = GREEN;
    DrawRectangleV(p->pos, p->size, color);
    // draw face
    Vector2 pos = p->pos;;
    Vector2 size = Vector2Scale(p->size, 0.3);
    switch (p->dir) {
        case UP:
            pos.x += p->size.x/2 - size.x/2;
            pos.y -= size.y;
            break;
        case DOWN:
            pos.x += p->size.x/2 - size.x/2;
            pos.y += p->size.y;
            break;
        case LEFT:
            pos.x -= size.x;
            pos.y += p->size.y/2 - size.x/2;
            break;
        case RIGHT:
            pos.x += p->size.x;
            pos.y += p->size.y/2 - size.y/2;
            break;
    }
    DrawRectangleV(pos, size, RED);
}

void player_free(Player *p) {
    free(p);
}
