#include "status.h"
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include "util.h"

struct StatusKnockback {
    Vector2 knockback;
    float distance;
    float angle;
    int duration;
    int time_elapsed;
};

StatusKnockback *status_knockback_create(float distance, float angle, int duration) {
    StatusKnockback *s_kb = malloc(sizeof(StatusKnockback));
    util_check_alloc(s_kb);
    s_kb->knockback = (Vector2){0};
    s_kb->distance = distance;
    s_kb->angle = angle;
    s_kb->duration = duration;
    s_kb->time_elapsed = 0;
    return s_kb;
}

#define STATUS_KNOCKBACK_DECAY_FACTOR 1.5
void status_knockback_update(StatusKnockback **s_kb_ptr) {
    StatusKnockback *s_kb = *s_kb_ptr;
    float t = (float)s_kb->time_elapsed / s_kb->duration;
    float decay_rate = exp(-STATUS_KNOCKBACK_DECAY_FACTOR * t);
    float d = decay_rate * (s_kb->distance / s_kb->duration);
    s_kb->knockback = Vector2Rotate((Vector2){0, d}, s_kb->angle);
    s_kb->time_elapsed++;
    if (s_kb->time_elapsed >= s_kb->duration) {
        free(s_kb);
        *s_kb_ptr = NULL;
    }
}

Vector2 status_knockback_get(StatusKnockback *s_kb) {
    if (s_kb == NULL)
        return (Vector2){0};
    else
        return s_kb->knockback;
}

struct StatusRecoil {
    float kickback_max;
    float kickback_curr;
    int duration;
    int dir;
};

StatusRecoil *status_recoil_create(float kickback, int duration) {
    StatusRecoil *s_r = malloc(sizeof(StatusRecoil));
    util_check_alloc(s_r);
    s_r->kickback_max = kickback;
    s_r->kickback_curr = 0;
    s_r->duration = duration;
    s_r->dir = -1;
    return s_r;
}

#define STATUS_RECOIL_KICKBACK_FACTOR 4
#define STATUS_RECOIL_RECOVER_FACTOR 2
void status_recoil_update(StatusRecoil **s_r_ptr) {
    StatusRecoil *s_r = *s_r_ptr;
    float d = 0;
    int *dir = &s_r->dir;
    if (*dir == -1)
        d = STATUS_RECOIL_KICKBACK_FACTOR * (s_r->kickback_max / s_r->duration);
    if (*dir == 1)
        d = -STATUS_RECOIL_RECOVER_FACTOR * (s_r->kickback_max / s_r->duration);
    s_r->kickback_curr += d;
    if(s_r->kickback_curr >= s_r->kickback_max)
        *dir = 1;
    else if(s_r->kickback_curr <= 0) {
        free(s_r);
        *s_r_ptr = NULL;
    }
}

float status_recoil_get(StatusRecoil *s_r) {
    if (s_r == NULL)
        return 0;
    else
        return s_r->kickback_curr;
}
