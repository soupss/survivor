#include <stdlib.h>
#include <stdio.h>
#include "mob.h"
#include <raymath.h>
#include "util.h"

#define MOB_MOVE_SPEED 1.2
#define MOB_COLOR RED
#define MOB_MAX_HP 10

struct Mob {
    Vector2 pos;
    float radius;
    Vector2 dir;
    int hit_points;
};

Mob *mob_create(Vector2 pos) {
    Mob *m = malloc(sizeof(Mob));
    check_alloc(m);
    m->pos = pos;
    m->radius = MOB_RADIUS;
    m->dir = (Vector2){0, -1};
    m->hit_points = MOB_MAX_HP;
    return m;
}

void mob_free(Mob *m) {
    //TODO: explosion
    free(m);
}

void mob_update(Mob *m, Vector2 target) {
    //TODO: rotate instead of snap
    m->dir = Vector2Normalize(Vector2Subtract(target, m->pos));
    m->pos = Vector2Add(m->pos ,Vector2Scale(m->dir, MOB_MOVE_SPEED));
}

void mob_draw(Mob *m) {
    double angle = -RAD2DEG * Vector2Angle(m->dir, (Vector2){1, 0});
    DrawPoly(m->pos, 3, m->radius, angle, MOB_COLOR);
}

Vector2 mob_get_pos(Mob *m) {
    return m->pos;
}

bool mob_is_dead(Mob *m) {
    if (m->hit_points <= 0) return true;
    else return false;
}

void mob_reduce_hp(Mob *m, int hp) {
    m->hit_points -= hp;
}
