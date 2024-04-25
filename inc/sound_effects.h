#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H

#include <raylib.h>

typedef struct {
    Sound shoot;
    Sound hurt;
    Sound hit;
    Sound exp;
} SoundEffects;

SoundEffects *sound_effects_create();
void sound_effects_destroy(SoundEffects *sfx);

#endif
