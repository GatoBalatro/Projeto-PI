#include <raylib.h>
#include "game.h" 

typedef struct {
    Vector2 position;
    Vector2 speed;
    int width;
    int height;
} Player;

// Função principal do jogo 
void RunGame() {
    Player player = { {400, 300}, {2.0f, 2.0f}, 50, 50 };

    while (!WindowShouldClose()) {
        // movimento com WASD
        if (IsKeyDown(KEY_W)) player.position.y -= player.speed.y;
        if (IsKeyDown(KEY_S)) player.position.y += player.speed.y;
        if (IsKeyDown(KEY_A)) player.position.x -= player.speed.x;
        if (IsKeyDown(KEY_D)) player.position.x += player.speed.x;

        // Limites da tela 
        if (player.position.x <= 0) player.position.x = 0;
        if (player.position.x + player.width >= 800) player.position.x = 800 - player.width;
        if (player.position.y <= 0) player.position.y = 0;
        if (player.position.y + player.height >= 600) player.position.y = 600 - player.height;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Use WASD pra mover o bloco!", 10, 10, 20, DARKGRAY);
        DrawRectangleRec((Rectangle){player.position.x, player.position.y, player.width, player.height}, BLUE);
        DrawText("raylib + multi-file works!", 190, 550, 20, BLACK);
        EndDrawing();
    }
}