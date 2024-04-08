#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include <raymath.h>
#include "runtime_constants.h"

#define PLAYER_TURN_SPEED 0.15
#define PLAYER_COLOR GREEN
#define PLAYER_SHOT_DELAY 10

struct Player {
    Vector2 pos;
    int radius;
    Vector2 dir;
    Vector2 target_dir;
    int shot_delta;
};

Player *player_create(Vector2 pos) {
    Player *p = malloc(sizeof(Player));
    if (p == NULL) {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    p->pos = pos;
    p->radius = PLAYER_RADIUS;
    p->dir = (Vector2){0, 1};
    p->shot_delta = 0;
    return p;
}

void player_destroy(Player *p) {
    free(p);
}

void player_set_target_dir(Player *p, Vector2 dir) {
    p->target_dir = dir;
}

void player_move(Player *p) {
    if (Vector2Length(p->target_dir) > 0) {
        Vector2 dir_normal = {p->dir.y, -p->dir.x};
        float rotation = Vector2DotProduct(dir_normal, p->target_dir);
        if (rotation > 0) p->dir = Vector2Rotate(p->dir, -PLAYER_TURN_SPEED);
        else p->dir = Vector2Rotate(p->dir, PLAYER_TURN_SPEED);
        //TODO: make dir converge fully with target dir
        p->pos = Vector2Add(p->pos, p->dir);
    }
}

void player_shoot(Player *p, List *bs) {
    if (p->shot_delta >= PLAYER_SHOT_DELAY) {
        Vector2 barrel_end = Vector2Add(p->pos, Vector2Scale(p->dir, p->radius + BARREL_LENGTH - 6));
        Bullet *b = bullet_create(barrel_end, p->dir);
        list_insert(bs, b);
        p->shot_delta = 0;
    }
    else p->shot_delta++;
}

void player_draw(Player *p) {
    DrawCircleV(p->pos, p->radius, PLAYER_COLOR);
    Vector2 barrel_pos = Vector2Add(p->pos, Vector2Scale(p->dir, p->radius + BARREL_LENGTH/2 - 2));
    float barrel_rotation = -RAD2DEG * Vector2Angle(p->dir, (Vector2){0, -1});
    DrawRectanglePro(
            (Rectangle){barrel_pos.x, barrel_pos.y, BARREL_WIDTH, BARREL_LENGTH},
            (Vector2){BARREL_WIDTH/2, BARREL_LENGTH/2}, barrel_rotation, PLAYER_COLOR);
}
