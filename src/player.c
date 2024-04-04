#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include <raymath.h>
#include "bullet.h"

#define PLAYER_COLOR GREEN
#define PLAYER_RADIUS 15
#define PLAYER_TURN_SPEED 0.1

#define BARREL_WIDTH (2 * BULLET_RADIUS)
#define BARREL_LENGTH 11

struct Player {
    Vector2 pos;
    float radius;
    Vector2 dir;
};

Player* player_create(Vector2 pos) {
    Player *p = malloc(sizeof(Player));
    if (p) {
        p->pos = pos;
        p->radius = PLAYER_RADIUS;
        p->dir = (Vector2){0, 1};
    }
    return p;
}

void player_update(Player *p) {
    Vector2 target_dir = {0};
    if (IsKeyDown(KEY_UP)) target_dir.y -= 1;
    if (IsKeyDown(KEY_DOWN)) target_dir.y += 1;
    if (IsKeyDown(KEY_LEFT)) target_dir.x -= 1;
    if (IsKeyDown(KEY_RIGHT)) target_dir.x += 1;
    if (Vector2Length(target_dir) > 0) {
        Vector2 dir_normal = {p->dir.y, -p->dir.x};
        float rotation = Vector2DotProduct(dir_normal, target_dir);
        if (rotation > 0) p->dir = Vector2Rotate(p->dir, -PLAYER_TURN_SPEED);
        else p->dir = Vector2Rotate(p->dir, PLAYER_TURN_SPEED);
        //TODO: make dir converge fully with target dir
        p->pos = Vector2Add(p->pos, p->dir);
    }
}

void player_draw(Player *p) {
    DrawCircleV(p->pos, p->radius, PLAYER_COLOR);
    Vector2 barrel_pos = Vector2Add(p->pos, Vector2Scale(p->dir, p->radius + BARREL_LENGTH/2 - 2));
    float barrel_rotation = -RAD2DEG * Vector2Angle(p->dir, (Vector2){0, -1});
    DrawRectanglePro(
            (Rectangle){barrel_pos.x, barrel_pos.y, BARREL_WIDTH, BARREL_LENGTH},
            (Vector2){BARREL_WIDTH/2, BARREL_LENGTH/2}, barrel_rotation, PLAYER_COLOR);
}

void player_delete(Player *p) {
    free(p);
}
