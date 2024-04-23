#ifndef STATUS_H
#define STATUS_H

#include <raylib.h>

typedef struct StatusKnockback StatusKnockback;
StatusKnockback *status_knockback_create(float distance, float angle, int duration);
Vector2 status_knockback_update(StatusKnockback *kb);
int status_knockback_get_duration(StatusKnockback *kb);
int status_knockback_get_time_elapsed(StatusKnockback *kb);

#endif
