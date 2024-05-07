#include "weapon.h"
#include <stdlib.h>
#include <raymath.h>
#include "status.h"
#include "util.h"

void _weapon_machinegun_shoot(Weapon *w, List *bs, Vector2 turret_pos, float turret_rotation, SoundEffects *sfx);
void _weapon_machinegun_draw(Weapon *w, Sprites *ss);
void _weapon_gatling_draw(Weapon *w, Sprites *ss);

typedef struct {
    Vector2 pos;
    float rotation;
    int shot_delta;
    StatusRecoil *status_recoil;
} WeaponMachinegun;

Weapon *weapon_machinegun_create(Vector2 pos, float rotation) {
    Weapon *w = malloc(sizeof(Weapon));
    util_check_alloc(w);
    w->use = _weapon_machinegun_shoot;
    w->draw = _weapon_machinegun_draw;
    WeaponMachinegun *mg = malloc(sizeof(WeaponMachinegun));
    util_check_alloc(mg);
    mg->shot_delta = 0;
    mg->status_recoil = NULL;
    mg->pos = pos;
    mg->rotation = rotation;
    w->state = mg;
    return w;
}

Weapon *weapon_gatling_create(Vector2 pos, float rotation) {
    Weapon *w = malloc(sizeof(Weapon));
    util_check_alloc(w);
    w->use = _weapon_machinegun_shoot;
    w->draw = _weapon_gatling_draw;
    WeaponMachinegun *mg = malloc(sizeof(WeaponMachinegun));
    util_check_alloc(mg);
    mg->shot_delta = 0;
    mg->status_recoil = NULL;
    mg->pos = pos;
    mg->rotation = rotation;
    w->state = mg;
    return w;
}

void weapon_free(Weapon *w) {
    free(w->state);
    free(w);
}

#define WEAPON_MACHINEGUN_SHOOTDELAY 40
#define BULLET_SPREAD (PI / 32.0f)
void _weapon_machinegun_shoot(Weapon *w, List *bs, Vector2 turret_pos, float turret_rotation, SoundEffects *sfx) {
    WeaponMachinegun *mg = w->state;
    if (mg->shot_delta >= WEAPON_MACHINEGUN_SHOOTDELAY) {
        PlaySound(sfx->shoot);
        // Vector2 pos = Vector2Add(turret_pos, Vector2Scale(mg->pos, PIXEL_SIZE));
        Vector2 pos = Vector2Add(turret_pos, mg->pos);
        Vector2 bullet_dir = Vector2Rotate((Vector2){0, 1}, -DEG2RAD * (turret_rotation + mg->rotation));
        Vector2 bullet_start = Vector2Subtract(pos, bullet_dir);
        // float spread = ((float)rand() / RAND_MAX * BULLET_SPREAD) - BULLET_SPREAD / 2;
        // bullet_dir = Vector2Rotate(bullet_dir, spread);
        Bullet *b = bullet_create(bullet_start, bullet_dir);
        list_insert(bs, b);
        mg->shot_delta = 0;
    }
    else
        mg->shot_delta++;
}

void _weapon_machinegun_draw(Weapon *w, Sprites *ss) {
    WeaponMachinegun *mg = w->state;
    Texture2D sprite = ss->weapon_machinegun;
    Rectangle source = {0, 0, sprite.width, sprite.height};
    Rectangle dest = {TURRET_RT_MIDDLE + mg->pos.x, TURRET_RT_MIDDLE + mg->pos.y, sprite.width, sprite.height};
    Vector2 origin = {(sprite.width - 1) / 2, 0};
    DrawTexturePro(sprite, source, dest, origin, mg->rotation, WHITE);
}

void _weapon_gatling_draw(Weapon *w, Sprites *ss) {
    WeaponMachinegun *g = w->state;
    Texture2D sprite = ss->weapon_gatling;
    Rectangle s = {0, 0, sprite.width, sprite.height};
    Rectangle d = {TURRET_RT_MIDDLE + g->pos.x, TURRET_RT_MIDDLE + g->pos.y, sprite.width, sprite.height};
    Vector2 origin = {sprite.width / 2 + 1, 0};
    DrawTexturePro(sprite, s, d, origin, g->rotation, WHITE);
}
