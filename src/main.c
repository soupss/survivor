#include <raylib.h>
#include "player.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

void handle_input(Player *p);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tank Survival");
    SetTargetFPS(60);
    Vector2 mid = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    Player *p = player_create(mid);
    while (!WindowShouldClose()) {
        player_update(p);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            player_draw(p);
        EndDrawing();
    }
    CloseWindow();
    player_delete(p);
    return 0;
}
