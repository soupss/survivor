#include <stdlib.h>
#include "mob.h"
#include <raymath.h>
#include "util.h"

#define MOB_SPEED_INITIAL 1
#define MOB_COLOR RED

struct Mob {
    Vector2 pos;
    int radius;
    Vector2 dir;
    float speed;
};

Mob *mob_create(Vector2 pos) {
    Mob *m = malloc(sizeof(Mob));
    check_alloc(m);
    m->pos = pos;
    m->radius = MOB_RADIUS;
    m->dir = (Vector2){0, -1};
    m->speed = MOB_SPEED_INITIAL;
    return m;
}

void mob_free(Mob *m) {
    free(m);
}

void mob_update(Mob *m, Vector2 target) {
    //TODO: rotate instead of snap
    m->dir = Vector2Normalize(Vector2Subtract(target, m->pos));
    m->pos = Vector2Add(m->pos ,Vector2Scale(m->dir, m->speed));
}

void mob_draw(Mob *m) {
    double angle = RAD2DEG * Vector2Angle(m->dir, (Vector2){1, 0});
    DrawPoly(m->pos, 3, m->radius, angle, MOB_COLOR);
}
