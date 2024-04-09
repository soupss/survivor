#include <stdio.h>
#include <raylib.h>
#include "player.h"
#include "bullet.h"
#include "mob.h"
#include "list.h"
#include "util.h"

#define MAX_BULLETS_INITIAL 5
#define MAX_MOBS_INITIAL 5

void initialize(Player **p, List **bs, List **ms);
void terminate(Player *p, List *bs, List *ms);
void update(Player *p, List *bs, List *ms);
void draw(Player *p, List *bs, List *ms);
void handle_input(Player *p);
void spawn_mob(List *ms, Vector2 pos);

int main() {
    Player *p;
    List *bs;   // bullets
    List *ms;   // mobs
    initialize(&p, &bs, &ms);
    int dt;
    while (!WindowShouldClose()) {
        if (dt > 50) {
            spawn_mob(ms, (Vector2){50, 50});
            dt = 0;
        }
        else dt++;
        update(p, bs, ms);
        draw(p, bs, ms);
    }
    terminate(p, bs, ms);
    return 0;
}

void initialize(Player **p, List **bs, List **ms) {
    InitWindow(0, 0, "Tank Survival");
    ToggleFullscreen();
    init_constants(GetScreenWidth(), GetScreenHeight());
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(60);
    Vector2 mid = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    *p = player_create(mid);
    *bs = list_create(MAX_BULLETS_INITIAL);
    *ms = list_create(MAX_MOBS_INITIAL);
}

void terminate(Player *p, List *bs, List *ms) {
    CloseWindow();
    player_free(p);
    list_free(bs);
    list_free(ms);
}

void update(Player *p, List *bs, List *ms) {
    handle_input(p);
    player_move(p);
    player_shoot(p, bs);
    for (int i = 0; i < list_len(bs); i++) {
        Bullet *b = list_get(bs, i);
        if (bullet_out_of_bounds(b)) bullet_free(list_delete(bs, i));
        else bullet_update(b);
    }
    for (int i = 0; i < list_len(ms); i++) {
        mob_update(list_get(ms, i), player_get_pos(p));
    }
}

void draw(Player *p, List *bs, List *ms) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < list_len(bs); i++) {
        bullet_draw(list_get(bs, i));
    }
    for (int i = 0; i < list_len(ms); i++) {
        mob_draw(list_get(ms, i));
    }
    player_draw(p);
    EndDrawing();
}

void handle_input(Player *p) {
    Vector2 dir = {0};
    if (IsKeyDown(KEY_UP)) dir.y -= 1;
    if (IsKeyDown(KEY_DOWN)) dir.y += 1;
    if (IsKeyDown(KEY_LEFT)) dir.x -= 1;
    if (IsKeyDown(KEY_RIGHT)) dir.x += 1;
    player_set_dir_target(p, dir);
}

void spawn_mob(List *ms, Vector2 pos) {
    Mob *m = mob_create(pos);
    list_insert(ms, m);
}
