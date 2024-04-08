#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "player.h"
#include <raymath.h>
#include "runtime_constants.h"

#define PLAYER_TURN_SPEED 0.12
#define PLAYER_COLOR GREEN
#define PLAYER_SHOT_DELAY 60

struct Player {
    Vector2 pos;
    int radius;
    Vector2 dir;
    double speed;
    Vector2 dir_target;
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
    p->dir = (Vector2){0, -1};
    p->speed = 2.0;
    p->shot_delta = 0;
    return p;
}

void player_destroy(Player *p) {
    free(p);
}

void player_set_dir_target(Player *p, Vector2 dir) {
    p->dir_target = dir;
}

void player_move(Player *p) {
    if (Vector2Length(p->dir_target) > 0) {
        Vector2 normal = {p->dir.y, -p->dir.x};
        double angle = PLAYER_TURN_SPEED;
        float d = Vector2DotProduct(normal, p->dir_target);
        if (d > 0) angle *= -1;
        double angle_to_target = Vector2Angle(p->dir, p->dir_target);
        if (fabs(angle_to_target) < fabs(angle)) {
            p->dir = Vector2Rotate(p->dir, angle_to_target);
        } else {
            p->dir = Vector2Rotate(p->dir, angle);
        }
        p->pos = Vector2Add(p->pos, Vector2Scale(p->dir, p->speed));
    }
}

void player_shoot(Player *p, List *bs) {
    if (p->shot_delta >= PLAYER_SHOT_DELAY) {
        Vector2 bullet_start = Vector2Add(p->pos, Vector2Scale(p->dir, p->radius + BARREL_LENGTH - BULLET_RADIUS * 2 - (double)BARREL_LENGTH/20));
        Bullet *b = bullet_create(bullet_start, p->dir);
        list_insert(bs, b);
        p->shot_delta = 0;
    }
    else p->shot_delta++;
}

void player_draw(Player *p) {
    DrawCircleV(p->pos, p->radius, PLAYER_COLOR);
    Vector2 barrel_pos = Vector2Add(p->pos, Vector2Scale(p->dir, p->radius + BARREL_LENGTH/2 - (double)BARREL_LENGTH/20));
    float barrel_rotation = -RAD2DEG * Vector2Angle(p->dir, (Vector2){0, -1});
    Rectangle r = {barrel_pos.x, barrel_pos.y, BARREL_WIDTH, BARREL_LENGTH};
    Vector2 v = {BARREL_WIDTH/2, BARREL_LENGTH/2};
    DrawRectanglePro(r, v, barrel_rotation, PLAYER_COLOR);
}
