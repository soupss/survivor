#include "status.h"
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "util.h"

struct StatusKnockback {
    float distance;
    float angle;
    float duration;
    float time_elapsed;
};

typedef struct {
    Vector2 pos;
    StatusKnockback *status_knockback;
} _EntityStatusVec;

typedef struct {
    Rectangle rec;
    StatusKnockback *status_knockback;
} _EntityStatusRec;

StatusKnockback *status_knockback_create(float distance, float angle, float duration) {
    StatusKnockback *kb = malloc(sizeof(StatusKnockback));
    check_alloc(kb);
    kb->distance = distance;
    kb->angle = angle;
    kb->duration = duration;
    kb->time_elapsed = 0;
    return kb;
}

// pointer must have "Vector2 pos" field
#define STATUS_KNOCKBACK_DECAY_RATE 1.5
void status_knockback_handle_vec(void *p) {
    _EntityStatusVec *e = (_EntityStatusVec*)p;
    StatusKnockback *kb = e->status_knockback;
    float t = kb->time_elapsed / kb->duration;
    float d = exp(-STATUS_KNOCKBACK_DECAY_RATE * t) * (kb->distance / kb->duration);
    Vector2 knockback = Vector2Rotate((Vector2){0, d}, kb->angle);
    e->pos = Vector2Add(e->pos, knockback);
    kb->time_elapsed++;
    if (kb->time_elapsed >= kb->duration) {
        free(kb);
        e->status_knockback = NULL;
    }
}

// pointer must have "Rectangle rec" field
void status_knockback_handle_rec(void *p) {
    _EntityStatusRec *e = (_EntityStatusRec*)p;
    StatusKnockback *kb = e->status_knockback;
    float d = kb->distance / kb->duration;
    Vector2 knockback = Vector2Rotate((Vector2){0, d}, kb->angle);
    e->rec.x += knockback.x;
    e->rec.y += knockback.y;
    kb->time_elapsed++;
    if (kb->time_elapsed >= kb->duration) {
        free(kb);
        e->status_knockback = NULL;
    }
}
