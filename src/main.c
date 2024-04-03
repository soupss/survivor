#include <raylib.h>
#include "player.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

void handle_input(Player *p);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Vampire Survivors 2");
    SetTargetFPS(60);
    Vector2 pp = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    Vector2 ps = {50, 50};
    Player *p = player_create(pp, ps);
    while (!WindowShouldClose()) {
        player_update(p);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            player_draw(p);
        EndDrawing();
    }
    CloseWindow();
    player_free(p);
    return 0;
}
