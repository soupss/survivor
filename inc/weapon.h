#ifndef WEAPON_H
#define WEAPON_H

#include "sound_effects.h"
#include "sprites.h"

typedef struct Weapon Weapon;
struct Weapon {
    void (*use)(Weapon *w, SoundEffects *sfx);
    void (*draw)(Weapon *w, Sprites *ss);
    void *state;
};

Weapon *weapon_machinegun_create(Vector2 pos, float rotation);
Weapon *weapon_gatling_create(Vector2 pos, float rotation);
void weapon_free(Weapon *w);

#endif
