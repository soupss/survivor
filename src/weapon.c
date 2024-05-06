#include "weapon.h"
#include <stdlib.h>
#include <raymath.h>
#include "status.h"
#include "util.h"

void _weapon_machinegun_shoot(Weapon *w, SoundEffects *sfx);
void _weapon_machinegun_draw(Weapon *w, Sprites *ss);
void _weapon_gatling_shoot(Weapon *w, SoundEffects *sfx);
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
#define BULLET_SPREAD (PI / 16.0f)
void _weapon_machinegun_shoot(Weapon *w, SoundEffects *sfx) {
    WeaponMachinegun *mg = w->state;
    if (mg->shot_delta >= WEAPON_MACHINEGUN_SHOOTDELAY) {
        PlaySound(sfx->shoot);
        // shoot from end of barrel
    }
    else
        mg->shot_delta++;
}

void _weapon_machinegun_draw(Weapon *w, Sprites *ss) {
    WeaponMachinegun *mg = w->state;
    Texture2D sprite = ss->weapon_machinegun;
    Rectangle s = {0, 0, sprite.width, sprite.height};
    Rectangle d = {mg->pos.x, mg->pos.y, sprite.width, sprite.height};
    Vector2 origin = {sprite.width / 2, 0};
    DrawTexturePro(sprite, s, d, origin, mg->rotation, WHITE);
}

void _weapon_gatling_draw(Weapon *w, Sprites *ss) {
    WeaponMachinegun *g = w->state;
    Texture2D sprite = ss->weapon_gatling;
    Rectangle s = {0, 0, sprite.width, sprite.height};
    Rectangle d = {g->pos.x, g->pos.y, sprite.width, sprite.height};
    Vector2 origin = {sprite.width / 2, 0};
    DrawTexturePro(sprite, s, d, origin, g->rotation, WHITE);
}
