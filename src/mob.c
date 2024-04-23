#include "mob.h"
#include <stdlib.h>
#include <stdio.h>
#include <raymath.h>
#include "status.h"
#include "util.h"

void _mob_move(Mob *m, Vector2 target, List *ms);
Vector2 _mob_separation_calculate(Mob *m, List *ms);
void _mob_status_knockback_handle(Mob *m);

struct Mob {
    Vector2 pos;
    StatusKnockback *status_knockback;
    Vector2 dir;
    int hit_points;
    float mouth_size;
    int mouth_dir;
    bool can_attack;
};

#define MOB_MAX_HP 10
#define MOB_MOUTH_SIZE 50
Mob *mob_create(Vector2 pos) {
    Mob *m = malloc(sizeof(Mob));
    util_check_alloc(m);
    m->pos = pos;
    m->dir = (Vector2){0, 1};
    m->hit_points = MOB_MAX_HP;
    m->mouth_size = MOB_MOUTH_SIZE;
    m->mouth_dir = -1;
    m->status_knockback = NULL;
    m->can_attack = false;
    return m;
}

void mob_destroy(Mob *m) {
    //TODO: explosion
    free(m);
}

#define MOB_MOUTH_SPEED 2
void mob_update(Mob *m, Vector2 target, List *ms) {
    StatusKnockback *kb = m->status_knockback;
    if (kb != NULL) {
        int duration = status_knockback_get_duration(kb);
        int time_elapsed = status_knockback_get_time_elapsed(kb);
        if (time_elapsed < duration) {
            Vector2 knockback = status_knockback_update(kb);
            m->pos = Vector2Add(m->pos, knockback);
        }
        else {
            free(kb);
            m->status_knockback = NULL;
        }
    }
    else {
        _mob_move(m, target, ms);
        m->mouth_size += m->mouth_dir * MOB_MOUTH_SPEED;
        if (m->mouth_size <= 0) {
            m->mouth_dir = 1;
            m->can_attack = false;
        }
        else if (m->mouth_size >= MOB_MOUTH_SIZE) {
            m->mouth_dir = -1;
            m->can_attack = true;
        }
    }
}

#define MOB_COLOR RED
void mob_draw(Mob *m) {
    float angle = -RAD2DEG * Vector2Angle(m->dir, (Vector2){1, 0});
    DrawCircleV(m->pos, MOB_RADIUS, MOB_COLOR);
    DrawCircleSector(m->pos, MOB_RADIUS, angle - m->mouth_size, angle + m->mouth_size, 365, BACKGROUND_COLOR);
}

#define MOB_MOVE_SPEED 1.4
void _mob_move(Mob *m, Vector2 target, List *ms) {
    //TODO: rotate instead of snap
    Vector2 steer = util_separation_from_mobs_v(m->pos, MOB_RADIUS, ms, 0.5);
    Vector2 d_vec = Vector2Subtract(target, m->pos);
    m->dir = Vector2Normalize(d_vec);
    float d = Vector2Length(d_vec);
    if (d > MOB_RADIUS + TANK_HITBOX_RADIUS)
        steer = Vector2Add(m->dir, steer);
    m->pos = Vector2Add(m->pos ,Vector2Scale(steer, MOB_MOVE_SPEED));
}

#define MOB_ATTACK_DAMAGE 2
int mob_attack(Mob *m) {
    if (m->can_attack) {
        m->can_attack = false;
        return MOB_ATTACK_DAMAGE;
    }
    else
        return 0;
}

void mob_hp_reduce(Mob *m, int hp) {
    m->hit_points -= hp;
}

void mob_set_status_knockback(Mob *m, float distance, float angle, int duration) {
    m->status_knockback = status_knockback_create(distance, angle, duration);
}

Vector2 mob_get_pos(Mob *m) {
    return m->pos;
}

Vector2 mob_get_dir(Mob *m) {
    return m->dir;
}

bool mob_is_dead(Mob *m) {
    if (m->hit_points <= 0)
        return true;
    else
        return false;
}
