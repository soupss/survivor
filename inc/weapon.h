#ifndef WEAPON_H
#define WEAPON_H

#include "sound_effects.h"
#include "sprites.h"
#include "list.h"

typedef struct Weapon Weapon;
struct Weapon {
    void (*use)(Weapon *w, List *bs, Vector2 turret_pos, float turret_rotation, SoundEffects *sfx);
    void (*draw)(Weapon *w, Sprites *ss);
    void *state;
};

Weapon *weapon_machinegun_create(Vector2 pos, float rotation);
Weapon *weapon_gatling_create(Vector2 pos, float rotation);
void weapon_free(Weapon *w);

#endif
