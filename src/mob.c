#include <stdlib.h>
#include <stdio.h>
#include "mob.h"
#include <raymath.h>
#include "util.h"

#define MOB_MOVE_SPEED 1.75
#define MOB_COLOR RED
#define MOB_MAX_HP 10
#define MOB_MOUTH_SIZE 50
#define MOB_MOUTH_SPEED 2

void _mob_move(Mob *m, Vector2 target, List *ms);
Vector2 _mob_separation_calculate(Mob *m, List *ms);
typedef struct _StatusKnockback _StatusKnockback;
_StatusKnockback *_status_knockback_create(float distance, float angle, float duration);
void _status_knockback_update(Mob *m);

struct Mob {
    Vector2 pos;
    Vector2 dir;
    int hit_points;
    float mouth_size;
    int mouth_dir;
    _StatusKnockback *status_knockback;
};

struct _StatusKnockback {
    float distance;
    float angle;
    float duration;
    float time_elapsed;
};

Mob *mob_create(Vector2 pos) {
    Mob *m = malloc(sizeof(Mob));
    check_alloc(m);
    m->pos = pos;
    m->dir = (Vector2){0, 1};
    m->hit_points = MOB_MAX_HP;
    m->mouth_size = MOB_MOUTH_SIZE;
    m->mouth_dir = -1;
    m->status_knockback = NULL;
    return m;
}

void mob_free(Mob *m) {
    //TODO: explosion
    free(m);
}

void mob_update(Mob *m, Vector2 target, List *ms) {
    //TODO: rotate instead of snap
    //TODO: handle collision between mobs
    if (m->status_knockback != NULL)
        _status_knockback_update(m);
    else
        _mob_move(m, target, ms);
}

void _mob_move(Mob *m, Vector2 target, List *ms) {
    m->dir = Vector2Normalize(Vector2Subtract(target, m->pos));
    Vector2 separate = _mob_separation_calculate(m, ms);
    Vector2 steer = Vector2Add(m->dir, separate);
    m->pos = Vector2Add(m->pos ,Vector2Scale(steer, MOB_MOVE_SPEED));
}

Vector2 _mob_separation_calculate(Mob *m, List *ms) {
    Vector2 steer = {0};
    int count = 0;
    for (int i = 0; i < list_len(ms); i++) {
        Mob *other = list_get(ms, i);
        if (m == other)
            continue;
        Vector2 diff = Vector2Subtract(m->pos, other->pos);
        float d = Vector2Length(diff);
        if (d < MOB_RADIUS * 2) {
            float separation = (0.5 * MOB_RADIUS) / d;
            diff = Vector2Scale(Vector2Normalize(diff), separation);
            steer = Vector2Add(steer, diff);
            count++;
        }
    }
    if (count > 0) {
        steer = Vector2Scale(steer, 1 / (float)count);
    }
    return steer;
}

void mob_draw(Mob *m) {
    m->mouth_size += m->mouth_dir * MOB_MOUTH_SPEED;
    if (m->mouth_size <= 0 || m->mouth_size >= MOB_MOUTH_SIZE)
        m->mouth_dir *= -1;
    float angle = -RAD2DEG * Vector2Angle(m->dir, (Vector2){1, 0});
    DrawCircleV(m->pos, MOB_RADIUS, MOB_COLOR);
    DrawCircleSector(m->pos, MOB_RADIUS, angle - m->mouth_size, angle + m->mouth_size, 365, BACKGROUND_COLOR);
}

Vector2 mob_get_pos(Mob *m) {
    return m->pos;
}

bool mob_is_dead(Mob *m) {
    if (m->hit_points <= 0)
        return true;
    else
        return false;
}

void mob_reduce_hp(Mob *m, int hp) {
    m->hit_points -= hp;
}

void mob_apply_knockback(Mob *m, float distance, float angle, float duration) {
    m->status_knockback = _status_knockback_create(distance, angle, duration);
}

_StatusKnockback *_status_knockback_create(float distance, float angle, float duration) {
    _StatusKnockback *kb = malloc(sizeof(_StatusKnockback));
    check_alloc(kb);
    kb->distance = distance;
    kb->angle = angle;
    kb->duration = duration;
    kb->time_elapsed = 0;
    return kb;
}

void _status_knockback_update(Mob *m) {
    _StatusKnockback *kb = m->status_knockback;
    float d = kb->distance / kb->duration;
    Vector2 knockback = Vector2Rotate((Vector2){0, d}, kb->angle);
    m->pos = Vector2Add(m->pos, knockback);
    kb->time_elapsed++;
    if (kb->time_elapsed >= kb->duration) {
        free(kb);
        m->status_knockback = NULL;
    }
}

#define EXP_SPEED 5

struct Exp {
    Vector2 pos;
    float exp_points;
};

Exp *exp_create(Vector2 pos) {
    Exp *xp = malloc(sizeof(Exp));
    check_alloc(xp);
    xp->pos = pos;
    xp->exp_points = 1.0;
    return xp;
}

void exp_free(Exp *xp) {
    free(xp);
}

void exp_update(Exp *xp, Vector2 target) {
    Vector2 diff = Vector2Subtract(target, xp->pos);
    float range = Vector2Length(diff);
    Vector2 dir = Vector2Normalize(diff);
    if (range <= EXP_RANGE)
        xp->pos = Vector2Add(xp->pos, Vector2Scale(dir, EXP_SPEED));
}

void exp_draw(Exp *xp) {
    DrawCircleGradient(xp->pos.x, xp->pos.y, EXP_RADIUS, RAYWHITE, EXP_COLOR);
}

Vector2 exp_get_pos(Exp *xp) {
    return xp->pos;
}

float exp_get_points(Exp *xp) {
    return xp->exp_points;
}
