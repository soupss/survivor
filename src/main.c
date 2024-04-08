#include <stdio.h>
#include <raylib.h>
#include "player.h"
#include "bullet.h"
#include "list.h"
#include "runtime_constants.h"

#define MAX_BULLETS_INITIAL 5

void initialize(Player **p, List **bs);
void terminate(Player *p, List *bs);
void update(Player *p, List *bs);
void draw(Player *p, List *bs);
void handle_input(Player *p);

int main() {
    Player *p;
    List *bs;
    initialize(&p, &bs);
    while (!WindowShouldClose()) {
        update(p, bs);
        draw(p, bs);
    }
    terminate(p, bs);
    return 0;
}

void initialize(Player **p, List **bs) {
    InitWindow(0, 0, "Tank Survival");
    ToggleFullscreen();
    init_constants(GetScreenWidth(), GetScreenHeight());
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTargetFPS(60);
    Vector2 mid = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    *p = player_create(mid);
    *bs = list_create(MAX_BULLETS_INITIAL);
}

void terminate(Player *p, List *bs) {
    CloseWindow();
    player_destroy(p);
    list_destroy(bs);
}

void update(Player *p, List *bs) {
    handle_input(p);
    player_move(p);
    player_shoot(p, bs);
    for (int i = 0; i < list_len(bs); i++) {
        bullet_update(list_get(bs, i));
    }
}

void draw(Player *p, List *bs) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i = 0; i < list_len(bs); i++) {
        bullet_draw(list_get(bs, i));
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
    player_set_target_dir(p, dir);
}
