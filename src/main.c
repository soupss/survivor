#include <stdio.h>
#include <raylib.h>
#include "tank.h"
#include "bullet.h"
#include "mob.h"
#include "list.h"
#include "util.h"

#define MAX_BULLETS_INITIAL 5
#define MAX_MOBS_INITIAL 5

void initialize(Tank **t, List **bs, List **ms);
void terminate(Tank *t, List *bs, List *ms);
void update(Tank *t, List *bs, List *ms);
void draw(Tank *t, List *bs, List *ms);
void handle_input(Tank *t);
void spawn_mob(List *ms, Vector2 pos);

int main() {
    Tank *t;
    List *bs;   // bullets
    List *ms;   // mobs
    initialize(&t, &bs, &ms);
    int dt;
    while (!WindowShouldClose()) {
        if (dt > 50) {
            spawn_mob(ms, (Vector2){1900, 1000});
            dt = 0;
        }
        else dt++;
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
    *t = tank_create(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    *bs = list_create(MAX_BULLETS_INITIAL);
    *ms = list_create(MAX_MOBS_INITIAL);
}

void terminate(Tank *t, List *bs, List *ms) {
    CloseWindow();
    tank_free(t);
    list_free(bs);
    list_free(ms);
}

void update(Tank *t, List *bs, List *ms) {
    handle_input(t);
    for (int i = 0; i < list_len(bs); i++) {
        Bullet *b = list_get(bs, i);
        if (bullet_out_of_bounds(b)) bullet_free(list_delete(bs, i));
        else bullet_update(b);
    }
    for (int i = 0; i < list_len(ms); i++) {
        mob_update(list_get(ms, i), tank_get_pos(t));
    }
}

void draw(Tank *t, List *bs, List *ms) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < list_len(bs); i++) {
        bullet_draw(list_get(bs, i));
    }
    // for (int i = 0; i < list_len(ms); i++) {
    //     mob_draw(list_get(ms, i));
    // }
    tank_draw(t);
    EndDrawing();
}

void handle_input(Tank *t) {
    bool hull_rotating = false;
    if (IsKeyDown(KEY_D)) {
        tank_rotate(t, 1);
        hull_rotating = true;
    }
    if (IsKeyDown(KEY_A)) {
        tank_rotate(t, -1);
        hull_rotating = true;
    }
    if (!hull_rotating){
        if (IsKeyDown(KEY_W)) tank_move(t, 1);
        else if (IsKeyDown(KEY_S)) tank_move(t, -1);
    }
    if (IsKeyDown(KEY_RIGHT)) tank_turret_rotate(t, 1);
    if (IsKeyDown(KEY_LEFT)) tank_turret_rotate(t, -1);
}

void spawn_mob(List *ms, Vector2 pos) {
    Mob *m = mob_create(pos);
    list_insert(ms, m);
}
