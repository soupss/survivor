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

#define MAX_BULLETS_INITIAL 5
#define MAX_MOBS_INITIAL 5
#define MAX_EXPS_INITIAL 5
#define MOB_SPAWN_DELAY 200
#define TANK_BULLET_DAMAGE 4
#define EXP_MAX 15

void initialize(Tank **t, List **bs, List **ms, List **xps, float *exp);
void update(Tank *t, List *bs, List *ms, List *xps, float *exp);
void draw(Tank *t, List *bs, List *ms, List *xps, float exp);
void terminate(Tank *t, List *bs, List *ms, List *xps);
void handle_input(Tank *t);
void spawn_mobs(List *ms);
void handle_collision_mob_bullet(List *bs, List *ms);
void handle_collision_exp_tank(List *xps, Tank *t, float *total_exp);
void draw_exp_bar(float exp);

int main() {
    Tank *t;
    List *bs;   // bullets
    List *ms;   // mobs
    List *xps;
    float exp;
    initialize(&t, &bs, &ms, &xps, &exp);
    while (!WindowShouldClose()) {
        update(t, bs, ms, xps, &exp);
        draw(t, bs, ms, xps, exp);
    }
    terminate(t, bs, ms, xps);
    return 0;
}

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
    *exp = 0.0;
}

void update(Tank *t, List *bs, List *ms, List *xps, float *exp) {
    handle_input(t);
    tank_update(t);
    tank_shoot(t, bs);
    spawn_mobs(ms);
    for (int i = 0; i < list_len(bs); i++) {
        Bullet *b = list_get(bs, i);
        if (bullet_out_of_bounds(b))
            bullet_free(list_delete(bs, i));
        else
            bullet_update(b);
    }
    for (int i = 0; i < list_len(ms); i++) {
        Mob *m = list_get(ms, i);
        if (mob_is_dead(m)) {
            Exp *xp = exp_create(mob_get_pos(m));
            list_insert(xps, xp);
            mob_free(list_delete(ms, i));
        }
        else
            mob_update(list_get(ms, i), tank_get_pos(t));
    }
    for (int i = 0; i < list_len(xps); i++) {
        exp_update(list_get(xps, i), tank_get_pos(t));
    }
    handle_collision_mob_bullet(bs, ms);
    handle_collision_exp_tank(xps, t, exp);
}

void draw(Tank *t, List *bs, List *ms, List *xps, float exp) {
    BeginDrawing();
    ClearBackground(BACKGROUND_COLOR);
    for (int i = 0; i < list_len(bs); i++)
        bullet_draw(list_get(bs, i));
    for (int i = 0; i < list_len(xps); i++)
        exp_draw(list_get(xps, i));
    for (int i = 0; i < list_len(ms); i++)
        mob_draw(list_get(ms, i));
    tank_draw(t);
    draw_exp_bar(exp);
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

void spawn_mobs(List *ms) {
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

void handle_collision_mob_bullet(List *bs, List *ms) {
    //TODO: precise collision
    for (int i = 0; i < list_len(ms); i++) {
        for (int y = 0; y < list_len(bs); y++) {
            Mob *m = list_get(ms, i);
            Bullet *b = list_get(bs, y);
            if (CheckCollisionCircles(mob_get_pos(m), MOB_RADIUS, bullet_get_pos(b), BULLET_RADIUS)) {
                mob_reduce_hp(m, TANK_BULLET_DAMAGE);
                Vector2 knockback_vector = Vector2Negate(bullet_get_dir(b));
                float knockback_angle = Vector2Angle((Vector2){0, 1}, knockback_vector);
                mob_apply_knockback(m, 15, knockback_angle);
                bullet_free(list_delete(bs, y));
                i--;
                y--;
                break;
            }
        }
    }
}

void handle_collision_exp_tank(List *xps, Tank *t, float *exp) {
    for (int i = 0; i < list_len(xps); i++) {
        Exp *xp = list_get(xps, i);
        if (CheckCollisionCircles(exp_get_pos(xp), EXP_RADIUS, tank_get_pos(t), TANK_TURRET_RADIUS)) {
            xp = list_delete(xps, i);
            (*exp) += exp_get_points(xp);
            exp_free(xp);
        }
    }
}

void draw_exp_bar(float exp) {
    float bar_width = (exp / EXP_MAX) * EXPBAR_WIDTH;
    DrawRectangle(0, SCREEN_HEIGHT - EXPBAR_HEIGHT + 1, bar_width, EXPBAR_HEIGHT, EXP_COLOR);
    //TODO: glow effect
}
