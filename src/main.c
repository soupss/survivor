#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include "tank.h"
#include "bullet.h"
#include "mob.h"
#include "exporb.h"
#include "list.h"
#include "util.h"
#include "sound_effects.h"
#include "sprites.h"

void initialize(Tank **t, List **bs, List **ms, List **xps, float *exp, SoundEffects **sfx, Sprites **ss);
void update(Tank *t, List *bs, List *ms, List *xps, float *exp, SoundEffects *sfx);
void draw(Tank *t, List *bs, List *ms, List *xps, float exp, Sprites *ss);
void terminate(Tank *t, List *bs, List *ms, List *xps, SoundEffects *sfx, Sprites *ss);
void input_handle(Tank *t);
void spawn_mob(List *ms);
void collision_handle_tank(Tank *t, List *ms, SoundEffects *sfx);
void collision_handle_bullet(List *bs, List *ms, List *xps, SoundEffects *sfx);
void collision_handle_exporb(List *xps, Tank *t, float *total_exp, SoundEffects *sfx);
void draw_expbar(float exp);
void draw_hurtscreen(int tank_hp);
void draw_deathscreen();

int main() {
    Tank *t;
    List *bs;   // bullets
    List *ms;   // mobs
    List *xps;  // exp orbs
    float exp;
    SoundEffects *sfx;
    Sprites *ss;
    initialize(&t, &bs, &ms, &xps, &exp, &sfx, &ss);
    while (!WindowShouldClose()) {
        update(t, bs, ms, xps, &exp, sfx);
        draw(t, bs, ms, xps, exp, ss);
    }
    terminate(t, bs, ms, xps, sfx, ss);
    return 0;
}

#define FPS_TARGET 60
#define MAX_BULLETS_INITIAL 5
#define MAX_MOBS_INITIAL 5
#define MAX_EXPS_INITIAL 5
void initialize(Tank **t, List **bs, List **ms, List **xps, float *exp, SoundEffects **sfx, Sprites **ss) {
    InitWindow(0, 0, "Tank Survival");
    ToggleFullscreen();
    util_init_constants(GetScreenWidth(), GetScreenHeight());
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(FPS_TARGET);
    srand(time(NULL));
    *sfx = sound_effects_create();
    *ss = sprites_create();
    *t = tank_create(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    *bs = list_create(MAX_BULLETS_INITIAL);
    *ms = list_create(MAX_MOBS_INITIAL);
    *xps = list_create(MAX_EXPS_INITIAL);
    *exp = 0;
}

void update(Tank *t, List *bs, List *ms, List *xps, float *exp, SoundEffects *sfx) {
    if (!tank_is_dead(t)) {
        input_handle(t);
        tank_update(t, bs, ms, sfx);
        spawn_mob(ms);
    }
    for (int i = 0; i < list_len(bs); i++) {
        Bullet *b = list_get(bs, i);
        if (bullet_is_out_of_bounds(b) || bullet_get_damage(b) <= 0)
            bullet_destroy(list_delete(bs, i));
        else
            bullet_update(b);
    }
    for (int i = 0; i < list_len(ms); i++)
        mob_update(list_get(ms, i), tank_get_pos(t), ms);
    for (int i = 0; i < list_len(xps); i++)
        exporb_update(list_get(xps, i), tank_get_pos(t));
    collision_handle_bullet(bs, ms, xps, sfx);
    collision_handle_tank(t, ms, sfx);
    collision_handle_exporb(xps, t, exp, sfx);
}

void draw(Tank *t, List *bs, List *ms, List *xps, float exp, Sprites *ss) {
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    for (int i = 0; i < list_len(xps); i++)
        exporb_draw(list_get(xps, i), ss);
    for (int i = 0; i < list_len(ms); i++)
        mob_draw(list_get(ms, i), ss);
    for (int i = 0; i < list_len(bs); i++)
        bullet_draw(list_get(bs, i), ss);
    tank_draw(t, ss);
    draw_expbar(exp);
    draw_hurtscreen(tank_get_hp(t));
    if (tank_is_dead(t))
        draw_deathscreen();
    DrawFPS(1, 0);
    EndDrawing();
}

void terminate(Tank *t, List *bs, List *ms, List *xps, SoundEffects *sfx, Sprites *ss) {
    tank_destroy(t);
    list_destroy(bs);
    list_destroy(ms);
    list_destroy(xps);
    sound_effects_destroy(sfx);
    sprites_destroy(ss);
    CloseWindow();
}

void input_handle(Tank *t) {
    bool hull_rotating = false;
    bool turret_rotating = false;
    bool tank_moving = false;
    if (IsKeyDown(KEY_D)) {
        tank_hull_rotate(t, 1);
        hull_rotating = true;
    }
    if (IsKeyDown(KEY_A)) {
        tank_hull_rotate(t, -1);
        hull_rotating = true;
    }
    if (!hull_rotating){
        tank_hull_rotate(t, 0);
        if (IsKeyDown(KEY_W)) {
            tank_velocity_calculate(t, 1);
            tank_moving = true;
        }
        else if (IsKeyDown(KEY_S)) {
            tank_velocity_calculate(t, -1);
            tank_moving = true;
        }
    }
    if (!tank_moving)
        tank_velocity_calculate(t, 0);
    if (IsKeyDown(KEY_RIGHT)) {
        tank_turret_rotate(t, 1);
        turret_rotating = true;
    }
    if (IsKeyDown(KEY_LEFT)) {
        tank_turret_rotate(t, -1);
        turret_rotating = true;
    }
    if (!turret_rotating)
        tank_turret_rotate(t, 0);
}

#define MOB_SPAWN_DELAY 150
void spawn_mob(List *ms) {
    static int spawn_delta = MOB_SPAWN_DELAY;
    if (spawn_delta >= MOB_SPAWN_DELAY) {
        Vector2 pos = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT};
        switch (rand() % 4) {
            case 0:
                pos.x = 0 - MOB_HITBOX_RADIUS;
                break;
            case 1:
                pos.x = SCREEN_WIDTH + MOB_HITBOX_RADIUS;
                break;
            case 2:
                pos.y = 0 - MOB_HITBOX_RADIUS;
                break;
            case 3:
                pos.y = SCREEN_HEIGHT + MOB_HITBOX_RADIUS;
                break;
        }
        Mob *m = mob_create(pos);
        list_insert(ms, m);
        spawn_delta = 0;
    }
    else
        spawn_delta++;
}

#define MOB_KNOCKBACK_DISTANCE_FACTOR 2
#define MOB_KNOCKBACK_DURATION_FACTOR 1
void collision_handle_tank(Tank *t, List *ms, SoundEffects *sfx) {
    for (int i = 0; i < list_len(ms); i++) {
        Mob *m = list_get(ms, i);
        if (CheckCollisionCircles(tank_get_pos(t), TANK_HITBOX_RADIUS, mob_get_pos(m), MOB_HITBOX_RADIUS)) {
            int dmg = mob_attack(m);
            if (dmg != 0) {
                PlaySound(sfx->hurt);
                tank_hp_reduce(t, dmg);
                float kb_angle = Vector2Angle((Vector2){0, 1}, mob_get_dir(m));
                float kb_distance = dmg * MOB_KNOCKBACK_DISTANCE_FACTOR;
                int kb_duration = dmg * MOB_KNOCKBACK_DURATION_FACTOR;
                tank_set_status_knockback(t, kb_distance, kb_angle, kb_duration);
            }
        }
    }
}

#define BULLET_KNOCKBACK_DISTANCE_FACTOR 3
#define BULLET_KNOCKBACK_DURATION_FACTOR 1
void collision_handle_bullet(List *bs, List *ms, List *xps, SoundEffects *sfx) {
    for (int i = 0; i < list_len(ms); i++) {
        for (int y = 0; y < list_len(bs); y++) {
            Mob *m = list_get(ms, i);
            Bullet *b = list_get(bs, y);
            if (CheckCollisionCircles(mob_get_pos(m), MOB_HITBOX_RADIUS, bullet_get_pos(b), BULLET_PIXELWIDTH * PIXEL_SIZE)) {
                PlaySound(sfx->hit);
                int dmg = bullet_get_damage(b);
                mob_hp_reduce(m, dmg);
                float kb_angle = Vector2Angle((Vector2){0, 1}, bullet_get_dir(b));
                float kb_distance = dmg * BULLET_KNOCKBACK_DISTANCE_FACTOR;
                int kb_duration = round(dmg * BULLET_KNOCKBACK_DURATION_FACTOR);
                mob_set_status_knockback(m, kb_distance, kb_angle, kb_duration);
                if (mob_is_dead(m)) {
                    ExpOrb *xp = exporb_create(mob_get_pos(m));
                    list_insert(xps, xp);
                    mob_destroy(list_delete(ms, i));
                    i--;
                }
                bullet_destroy(list_delete(bs, y));
                y--;
                break;
            }
        }
    }
}

void collision_handle_exporb(List *xps, Tank *t, float *exp, SoundEffects *sfx) {
    for (int i = 0; i < list_len(xps); i++) {
        ExpOrb *xp = list_get(xps, i);
        if (CheckCollisionCircles(exporb_get_pos(xp), EXPORB_RADIUS, tank_get_pos(t), 1)) {
            PlaySound(sfx->exp);
            xp = list_delete(xps, i);
            (*exp) += exporb_get_points(xp);
            exporb_destroy(xp);
        }
    }
}

#define EXP_MAX 15
void draw_expbar(float exp) {
    float bar_width = (exp / EXP_MAX) * EXPBAR_WIDTH;
    DrawRectangle(0, SCREEN_HEIGHT - EXPBAR_HEIGHT + 1, bar_width, EXPBAR_HEIGHT, EXP_COLOR);
    //TODO: glow effect
}

#define HS_MAX_ALPHA 0.9
void draw_hurtscreen(int tank_hp) {
    float alpha = HS_MAX_ALPHA - HS_MAX_ALPHA * (tank_hp / (float)TANK_MAX_HP);
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(MAROON, alpha));
}

#define DS_ALPHA_DEFAULT -0.2
#define DS_FADEIN_SPEED 0.004
#define DS_FONT_SIZE 100
#define DS_FONT_SPACING 10
void draw_deathscreen() {
    char *msg = "YOU DIED";
    Vector2 msg_size = MeasureTextEx(GetFontDefault(), msg, DS_FONT_SIZE, DS_FONT_SPACING);
    Vector2 screen_mid = {SCREEN_WIDTH / (float)2, SCREEN_HEIGHT / (float)2};
    Vector2 pos = Vector2Subtract(screen_mid, Vector2Scale(msg_size, 0.5));
    static float alpha = DS_ALPHA_DEFAULT;
    DrawTextEx(GetFontDefault(), msg, pos, DS_FONT_SIZE, DS_FONT_SPACING, Fade(BLACK, alpha));
    if (alpha <= 1) {
        alpha += DS_FADEIN_SPEED;
    }
}
