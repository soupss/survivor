#include "exporb.h"
#include <stdlib.h>
#include <raymath.h>
#include "util.h"

struct ExpOrb {
    Vector2 pos;
    float exp_points;
};

#define EXPORB_EXP_POINTS 1
ExpOrb *exporb_create(Vector2 pos) {
    ExpOrb *xp = malloc(sizeof(ExpOrb));
    check_alloc(xp);
    xp->pos = pos;
    xp->exp_points = EXPORB_EXP_POINTS;
    return xp;
}

void exporb_destroy(ExpOrb *xp) {
    free(xp);
}

#define EXPORB_SPEED 5
void exporb_update(ExpOrb *xp, Vector2 target) {
    Vector2 diff = Vector2Subtract(target, xp->pos);
    float range = Vector2Length(diff);
    Vector2 dir = Vector2Normalize(diff);
    if (range <= EXPORB_PICKUPRANGE)
        xp->pos = Vector2Add(xp->pos, Vector2Scale(dir, EXPORB_SPEED));
}

void exporb_draw(ExpOrb *xp) {
    DrawCircleGradient(xp->pos.x, xp->pos.y, EXPORB_RADIUS, RAYWHITE, EXP_COLOR);
}

Vector2 exporb_get_pos(ExpOrb *xp) {
    return xp->pos;
}

float exporb_get_points(ExpOrb *xp) {
    return xp->exp_points;
}
