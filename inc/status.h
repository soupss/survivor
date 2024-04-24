#ifndef STATUS_H
#define STATUS_H

#include <raylib.h>

typedef struct StatusKnockback StatusKnockback;
StatusKnockback *status_knockback_create(float distance, float angle, int duration);
void status_knockback_update(StatusKnockback **s_kb_ptr);
Vector2 status_knockback_get(StatusKnockback *s_kb);

typedef struct StatusRecoil StatusRecoil;
StatusRecoil *status_recoil_create(float kickback, int duration);
void status_recoil_update(StatusRecoil **s_r_ptr);
float status_recoil_get(StatusRecoil *s_r);

#endif
