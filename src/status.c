#include "status.h"
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "util.h"

struct StatusKnockback {
    float distance;
    float angle;
    int duration;
    int time_elapsed;
};

StatusKnockback *status_knockback_create(float distance, float angle, int duration) {
    StatusKnockback *kb = malloc(sizeof(StatusKnockback));
    util_check_alloc(kb);
    kb->distance = distance;
    kb->angle = angle;
    kb->duration = duration;
    kb->time_elapsed = 0;
    return kb;
}

#define STATUS_KNOCKBACK_DECAY_RATE 1.5
Vector2 status_knockback_update(StatusKnockback *kb) {
    float t = (float)kb->time_elapsed / kb->duration;
    float d = exp(-STATUS_KNOCKBACK_DECAY_RATE * t) * (kb->distance / kb->duration);
    Vector2 knockback = Vector2Rotate((Vector2){0, d}, kb->angle);
    kb->time_elapsed++;
    return knockback;
}

int status_knockback_get_duration(StatusKnockback *kb) {
    return kb->duration;
}

int status_knockback_get_time_elapsed(StatusKnockback *kb) {
    return kb->time_elapsed;
}
