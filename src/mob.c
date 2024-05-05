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
    bool has_attacked;
};

#define MOB_MAX_HP 15
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
    m->has_attacked = false;
    return m;
}

void mob_destroy(Mob *m) {
    //TODO: death animation
    free(m);
}

#define MOB_MOUTH_SPEED 1
#define MOB_MOUTH_SIZE_ATTACK_FACTOR 0.6
void mob_update(Mob *m, Vector2 target, List *ms) {
    if (m->status_knockback != NULL)
        status_knockback_update(&m->status_knockback);
    if (m->status_knockback != NULL) {
        Vector2 knockback = status_knockback_get(m->status_knockback);
        m->pos = Vector2Add(m->pos, knockback);
    }
    else {
        _mob_move(m, target, ms);
        m->mouth_size += m->mouth_dir * MOB_MOUTH_SPEED;
        if (m->mouth_size <= 0) {
            m->mouth_dir = 1;
            m->can_attack = false;
            m->has_attacked = false;
        }
        else if (m->mouth_size >= MOB_MOUTH_SIZE)
            m->mouth_dir = -1;
        if (m->mouth_dir == -1 && m->mouth_size <= MOB_MOUTH_SIZE * MOB_MOUTH_SIZE_ATTACK_FACTOR && m->has_attacked == false)
            m->can_attack = true;
        else
            m->can_attack = false;
    }
}

void mob_draw(Mob *m, Sprites *ss) {
    Vector2 size = {SPIDER_PIXELWIDTH * PIXEL_SIZE, SPIDER_PIXELHEIGHT * PIXEL_SIZE};
    Rectangle rec_source = {0, 0, SPIDER_PIXELWIDTH, SPIDER_PIXELHEIGHT};
    Rectangle rec_dest = {m->pos.x, m->pos.y, size.x, size.y};
    Vector2 origin = {size.x / 2, size.y / 2};
    float rotation = -RAD2DEG * Vector2Angle(m->dir, (Vector2){0, 1});
    DrawTexturePro(ss->spider, rec_source, rec_dest, origin, rotation, WHITE);
}

#define MOB_MOVE_SPEED 1.4
void _mob_move(Mob *m, Vector2 target, List *ms) {
    //TODO: rotate instead of snap
    Vector2 steer = util_separation_from_mobs_v(m->pos, MOB_HITBOX_RADIUS, ms, 0.5);
    Vector2 d_vec = Vector2Subtract(target, m->pos);
    m->dir = Vector2Normalize(d_vec);
    float d = Vector2Length(d_vec);
    if (d > MOB_HITBOX_RADIUS + TANK_HITBOX_RADIUS)
        steer = Vector2Add(m->dir, steer);
    m->pos = Vector2Add(m->pos ,Vector2Scale(steer, MOB_MOVE_SPEED));
}

#define MOB_ATTACK_DAMAGE 4
int mob_attack(Mob *m) {
    if (m->can_attack) {
        m->has_attacked = true;
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
