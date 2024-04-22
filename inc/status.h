#ifndef STATUS_H
#define STATUS_H

typedef struct StatusKnockback StatusKnockback;
StatusKnockback *status_knockback_create(float distance, float angle, float duration);
void status_knockback_handle_vec(void *p); // pointer has a Vector2 pos field
void status_knockback_handle_rec(void *p); // pointer has a Rectangle rec field

#endif
