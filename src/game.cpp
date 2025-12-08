#include <raylib.h>
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "fase_corrida.h"
#include "menu.h"



// Main game loop (now clean and readable!)

void RunGame() {
    InitGame();

    float timer = 0.0f;
    Vector2 cameraOffset = {0, 0};
    float position_x_mais_longe = 0.0f;

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        timer += dt;

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentState) {

            case MENU:
                HandleInput();
                DrawMenu();       // <-- você tinha esquecido
                break;

            case FASE_CORRIDA:
                UpdateGame(dt);   // <-- atualiza player, obstáculos, animação, etc.
                DrawGame(cameraOffset, timer); 
                position_x_mais_longe = fmaxf(position_x_mais_longe, player.position.x);
                break;

            case FASE_NATACAO:
                fase_natacao();   // fase ainda WIP
                break;

            case FASE_CICLISMO:
                DrawText("FASE CICLISMO", 300, 300, 40, YELLOW);
                break;

            case GAME_OVER:
                DrawText("GAME OVER", 350, 350, 50, RED);
                break;
        }

        EndDrawing();
    }

    CleanupGame();
}

