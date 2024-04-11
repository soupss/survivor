#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <raylib.h>
#include "tank.h"
#include "bullet.h"
#include "mob.h"
#include "list.h"
#include "util.h"

#define MAX_BULLETS_INITIAL 5
#define MAX_MOBS_INITIAL 5
#define MOB_SPAWN_DELAY 200
#define TANK_BULLET_DAMAGE 4

void initialize(Tank **t, List **bs, List **ms);
void update(Tank *t, List *bs, List *ms);
void draw(Tank *t, List *bs, List *ms);
void terminate(Tank *t, List *bs, List *ms);
void handle_input(Tank *t);
void spawn_mobs(List *ms);
void handle_collision_mob_bullet(List *bs, List *ms);
bool handle_collision_mob_tank(List *ms, Tank *t);

int main() {
    Tank *t;
    List *bs;   // bullets
    List *ms;   // mobs
    initialize(&t, &bs, &ms);
    while (!WindowShouldClose()) {
        update(t, bs, ms);
        draw(t, bs, ms);
    }
    terminate(t, bs, ms);
    return 0;
}

void initialize(Tank **t, List **bs, List **ms) {
    InitWindow(0, 0, "Tank Survival");
    ToggleFullscreen();
    init_constants(GetScreenWidth(), GetScreenHeight());
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(60);
    srand(time(NULL));
    *t = tank_create(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    *bs = list_create(MAX_BULLETS_INITIAL);
    *ms = list_create(MAX_MOBS_INITIAL);
}

void update(Tank *t, List *bs, List *ms) {
    handle_input(t);
    tank_shoot(t, bs);
    spawn_mobs(ms);
    for (int i = 0; i < list_len(bs); i++) {
        Bullet *b = list_get(bs, i);
        if (bullet_out_of_bounds(b)) bullet_free(list_delete(bs, i));
        else bullet_update(b);
    }
    for (int i = 0; i < list_len(ms); i++) {
        Mob *m = list_get(ms, i);
        if (mob_is_dead(m)) mob_free(list_delete(ms, i));
        else mob_update(list_get(ms, i), tank_get_pos(t));
    }
    handle_collision_mob_bullet(bs, ms);
}

void draw(Tank *t, List *bs, List *ms) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < list_len(ms); i++) {
        mob_draw(list_get(ms, i));
    }
    for (int i = 0; i < list_len(bs); i++) {
        bullet_draw(list_get(bs, i));
    }
    tank_draw(t);
    EndDrawing();
}

void terminate(Tank *t, List *bs, List *ms) {
    CloseWindow();
    tank_free(t);
    list_free(bs);
    list_free(ms);
}

void handle_input(Tank *t) {
    bool hull_rotating = false;
    bool turret_rotating_cont = false;
    if (IsKeyDown(KEY_D)) {
        tank_hull_rotate(t, 1);
        hull_rotating = true;
    }
    if (IsKeyDown(KEY_A)) {
        tank_hull_rotate(t, -1);
        hull_rotating = true;
    }
    if (!hull_rotating){
        if (IsKeyDown(KEY_W)) tank_move(t, 1);
        else if (IsKeyDown(KEY_S)) tank_move(t, -1);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        tank_turret_rotate(t, 1);
        turret_rotating_cont = true;
    }
    if (IsKeyDown(KEY_LEFT)) {
        tank_turret_rotate(t, -1);
        turret_rotating_cont = true;
    }
    if (!turret_rotating_cont) tank_turret_rotate(t, 0);
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
    else spawn_delta++;
}

void handle_collision_mob_bullet(List *bs, List *ms) {
    //TODO: precise collision
    for (int i = 0; i < list_len(ms); i++) {
        for (int y = 0; y < list_len(bs); y++) {
            Mob *m = list_get(ms, i);
            Bullet *b = list_get(bs, y);
            if (CheckCollisionCircles(mob_get_pos(m), MOB_RADIUS * 0.8, bullet_get_pos(b), BULLET_RADIUS)) {
                mob_reduce_hp(m, TANK_BULLET_DAMAGE);
                bullet_free(list_delete(bs, y));
                i--;
                y--;
                break;
            }
        }
    }
}
