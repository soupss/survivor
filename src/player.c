#include <stdlib.h>
#include "player.h"

#define PLAYER_RADIUS 15
#define PLAYER_TURN_SPEED 0.1

struct Player {
    Vector2 pos;
    int radius;
    Vector2 current_dir;
};

Player* player_create(Vector2 pos) {
    Player *p = malloc(sizeof(Player));
    if (p) {
        p->pos = pos;
        p->radius = PLAYER_RADIUS;
        p->current_dir = (Vector2){0, 1};
    }
    return p;
}

void player_update(Player *p) {
    Vector2 tdir = {0, 0};
    if (IsKeyDown(KEY_UP)) tdir.y -= 1;
    if (IsKeyDown(KEY_DOWN)) tdir.y += 1;
    if (IsKeyDown(KEY_LEFT)) tdir.x -= 1;
    if (IsKeyDown(KEY_RIGHT)) tdir.x += 1;
    if (Vector2Length(tdir) > 0) {
        tdir = Vector2Normalize(tdir);
        p->current_dir = Vector2Lerp(p->current_dir, tdir, PLAYER_TURN_SPEED);
        DrawLineV(p->pos, Vector2Add(p->pos, Vector2Scale(p->current_dir, 100)), RED);
        p->pos = Vector2Add(p->pos, p->current_dir);
    }
}

void player_draw(Player *p) {
    DrawCircleV(p->pos, p->radius, GREEN);
}

void player_free(Player *p) {
    free(p);
}
