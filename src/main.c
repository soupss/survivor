#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include "tank.h"
#include "bullet.h"
#include "mob.h"
#include "list.h"
#include "util.h"

void initialize(Tank **t, List **bs, List **ms, List **xps, float *exp);
void update(Tank *t, List *bs, List *ms, List *xps, float *exp);
void draw(Tank *t, List *bs, List *ms, List *xps, float exp);
void terminate(Tank *t, List *bs, List *ms, List *xps);
void handle_input(Tank *t);
void spawn_mob(List *ms);
void handle_collision_tank(Tank *t, List *ms);
void handle_collision_bullet(List *bs, List *ms, List *xps);
void handle_collision_exporb(List *xps, Tank *t, float *total_exp);
void draw_exp_bar(float exp);
void draw_hurt_screen(int tank_hp);
void draw_death_screen();

int main() {
    Tank *t;
    List *bs;   // bullets
    List *ms;   // mobs
    List *xps;  // exp orbs
    float exp;
    initialize(&t, &bs, &ms, &xps, &exp);
    while (!WindowShouldClose()) {
        update(t, bs, ms, xps, &exp);
        draw(t, bs, ms, xps, exp);
    }
    terminate(t, bs, ms, xps);
    return 0;
}

#define MAX_BULLETS_INITIAL 5
#define MAX_MOBS_INITIAL 5
#define MAX_EXPS_INITIAL 5
void initialize(Tank **t, List **bs, List **ms, List **xps, float *exp) {
    InitWindow(0, 0, "Tank Survival");
    ToggleFullscreen();
    init_constants(GetScreenWidth(), GetScreenHeight());
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(60);
    srand(time(NULL));
    *t = tank_create(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    *bs = list_create(MAX_BULLETS_INITIAL);
    *ms = list_create(MAX_MOBS_INITIAL);
    *xps = list_create(MAX_EXPS_INITIAL);
    *exp = 0;
}

void update(Tank *t, List *bs, List *ms, List *xps, float *exp) {
    if (!tank_is_dead(t)) {
        handle_input(t);
        tank_move(t);
        tank_shoot(t, bs);
        spawn_mob(ms);
    }
    for (int i = 0; i < list_len(bs); i++) {
        Bullet *b = list_get(bs, i);
        if (bullet_out_of_bounds(b))
            bullet_free(list_delete(bs, i));
        else
            bullet_update(b);
    }
    for (int i = 0; i < list_len(ms); i++) {
        Mob *m = list_get(ms, i);
        mob_update(list_get(ms, i), tank_get_pos(t), ms);
    }
    for (int i = 0; i < list_len(xps); i++) {
        exp_update(list_get(xps, i), tank_get_pos(t));
    }
    handle_collision_bullet(bs, ms, xps);
    handle_collision_exporb(xps, t, exp);
    handle_collision_tank(t, ms);
}

void draw(Tank *t, List *bs, List *ms, List *xps, float exp) {
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    for (int i = 0; i < list_len(ms); i++)
        mob_draw(list_get(ms, i));
    for (int i = 0; i < list_len(bs); i++)
        bullet_draw(list_get(bs, i));
    for (int i = 0; i < list_len(xps); i++)
        exp_draw(list_get(xps, i));
    tank_draw(t);
    draw_exp_bar(exp);
    draw_hurt_screen(tank_get_hp(t));
    if (tank_is_dead(t))
        draw_death_screen();
    EndDrawing();
}

void terminate(Tank *t, List *bs, List *ms, List *xps) {
    CloseWindow();
    tank_free(t);
    list_free(bs);
    list_free(ms);
    list_free(xps);
}

void handle_input(Tank *t) {
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

#define MOB_SPAWN_DELAY 200
void spawn_mob(List *ms) {
    static int spawn_delta = 0;
    if (spawn_delta >= MOB_SPAWN_DELAY) {
        Vector2 pos = {rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT};
        switch (rand() % 4) {
            case 0:
                pos.x = 0 - MOB_RADIUS;
                break;
            case 1:
                pos.x = SCREEN_WIDTH + MOB_RADIUS;
                break;
            case 2:
                pos.y = 0 - MOB_RADIUS;
                break;
            case 3:
                pos.y = SCREEN_HEIGHT + MOB_RADIUS;
                break;
        }
        Mob *m = mob_create(pos);
        list_insert(ms, m);
        spawn_delta = 0;
    }
    else
        spawn_delta++;
}

void handle_collision_tank(Tank *t, List *ms) {
    for (int i = 0; i < list_len(ms); i++) {
        Mob *m = list_get(ms, i);
        float tank_hitbox_radius = TANK_TURRET_RADIUS * 1.2;
        if (CheckCollisionCircles(tank_get_pos(t), tank_hitbox_radius, mob_get_pos(m), MOB_RADIUS)) {
            int damage = mob_attack(m);
            if (damage != 0) {
                tank_reduce_hp(t, damage);
            }
        }
    }
}

#define BULLET_KNOCKBACK_DISTANCE_FACTOR 1.5
#define BULLET_KNOCKBACK_DURATION 3
void handle_collision_bullet(List *bs, List *ms, List *xps) {
    for (int i = 0; i < list_len(ms); i++) {
        for (int y = 0; y < list_len(bs); y++) {
            Mob *m = list_get(ms, i);
            Bullet *b = list_get(bs, y);
            if (CheckCollisionCircles(mob_get_pos(m), MOB_RADIUS, bullet_get_pos(b), BULLET_RADIUS)) {
                float dmg = bullet_get_damage(b);
                mob_reduce_hp(m, dmg);
                float knockback_angle = Vector2Angle((Vector2){0, 1}, bullet_get_dir(b));
                float knockback_distance = dmg * BULLET_KNOCKBACK_DISTANCE_FACTOR;
                mob_apply_knockback(m, knockback_distance, knockback_angle, BULLET_KNOCKBACK_DURATION);
                if (mob_is_dead(m)) {
                    ExpOrb *xp = exp_create(mob_get_pos(m));
                    list_insert(xps, xp);
                    mob_free(list_delete(ms, i));
                    i--;
                }
                bullet_free(list_delete(bs, y));
                y--;
                break;
            }
        }
    }
}

void handle_collision_exporb(List *xps, Tank *t, float *exp) {
    for (int i = 0; i < list_len(xps); i++) {
        ExpOrb *xp = list_get(xps, i);
        if (CheckCollisionCircles(exp_get_pos(xp), EXPORB_RADIUS, tank_get_pos(t), TANK_TURRET_RADIUS)) {
            xp = list_delete(xps, i);
            (*exp) += exp_get_points(xp);
            exp_free(xp);
        }
    }
}

#define EXP_MAX 15
void draw_exp_bar(float exp) {
    float bar_width = (exp / EXP_MAX) * EXPBAR_WIDTH;
    DrawRectangle(0, SCREEN_HEIGHT - EXPBAR_HEIGHT + 1, bar_width, EXPBAR_HEIGHT, EXP_COLOR);
    //TODO: glow effect
}

#define HS_MAX_ALPHA 0.9
void draw_hurt_screen(int tank_hp) {
    float alpha = HS_MAX_ALPHA - HS_MAX_ALPHA * (tank_hp / (float)TANK_MAX_HP);
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(MAROON, alpha));
}

#define DS_FADEIN_SPEED 0.003
#define DS_FONT_SIZE 100
#define DS_FONT_SPACING 10
void draw_death_screen() {
    char *msg = "YOU DIED";
    Vector2 msg_size = MeasureTextEx(GetFontDefault(), msg, DS_FONT_SIZE, DS_FONT_SPACING);
    Vector2 screen_mid = {SCREEN_WIDTH / (float)2, SCREEN_HEIGHT / (float)2};
    Vector2 pos = Vector2Subtract(screen_mid, Vector2Scale(msg_size, 0.5));
    static float alpha = -0.3;
    DrawTextEx(GetFontDefault(), msg, pos, DS_FONT_SIZE, DS_FONT_SPACING, Fade(BLACK, alpha));
    if (alpha <= 1) {
        alpha += DS_FADEIN_SPEED;
    }
}
