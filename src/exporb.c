#include "exporb.h"
#include <stdlib.h>
#include <raymath.h>
#include "util.h"

struct ExpOrb {
    Vector2 pos;
    float exp_points;
};

ExpOrb *exp_create(Vector2 pos) {
    ExpOrb *xp = malloc(sizeof(ExpOrb));
    check_alloc(xp);
    xp->pos = pos;
    xp->exp_points = 1.0;
    return xp;
}

void exp_free(ExpOrb *xp) {
    free(xp);
}

#define EXPORB_SPEED 5
void exp_update(ExpOrb *xp, Vector2 target) {
    Vector2 diff = Vector2Subtract(target, xp->pos);
    float range = Vector2Length(diff);
    Vector2 dir = Vector2Normalize(diff);
    if (range <= EXPORB_PICKUPRANGE)
        xp->pos = Vector2Add(xp->pos, Vector2Scale(dir, EXPORB_SPEED));
}

void exp_draw(ExpOrb *xp) {
    DrawCircleGradient(xp->pos.x, xp->pos.y, EXPORB_RADIUS, RAYWHITE, EXP_COLOR);
}

Vector2 exp_get_pos(ExpOrb *xp) {
    return xp->pos;
}

float exp_get_points(ExpOrb *xp) {
    return xp->exp_points;
}
