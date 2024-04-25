#include "sound_effects.h"
#include <stdlib.h>
#include "util.h"

SoundEffects *sound_effects_create() {
    InitAudioDevice();
    SoundEffects *sfx = malloc(sizeof(SoundEffects));
    util_check_alloc(sfx);
    sfx->shoot = LoadSound("resources/shoot.wav");
    sfx->hurt = LoadSound("resources/hurt.wav");
    sfx->hit = LoadSound("resources/hit.wav");
    sfx->exp = LoadSound("resources/exp.wav");
    return sfx;
}

void sound_effects_destroy(SoundEffects *sfx) {
    UnloadSound(sfx->shoot);
    free(sfx);
    CloseAudioDevice();
}
