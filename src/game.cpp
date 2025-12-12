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
    LoadMenuMusic();  // Carregar música do menu

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
                // Tocar música do menu se foi carregada
                if (musicLoaded && menuMusic.frameCount > 0) {
                    if (!IsMusicStreamPlaying(menuMusic)) {
                        PlayMusicStream(menuMusic);
                    }
                    UpdateMusicStream(menuMusic);  // Atualiza o stream de música
                }
                HandleInput();
                DrawMenu();       // <-- você tinha esquecido
                break;

            case FASE_CORRIDA:
                // Parar música do menu quando entrar na fase
                if (IsMusicStreamPlaying(menuMusic)) {
                    StopMusicStream(menuMusic);
                }
                UpdateGame(dt);   // <-- atualiza player, obstáculos, animação, etc.
                DrawGame(cameraOffset, timer); 
                position_x_mais_longe = fmaxf(position_x_mais_longe, player.position.x);
                break;

            case FASE_NATACAO:
                // Parar música do menu quando entrar na fase
                if (IsMusicStreamPlaying(menuMusic)) {
                    StopMusicStream(menuMusic);
                }
                fase_natacao();   // fase ainda WIP
                break;

            case FASE_CICLISMO:
                // Parar música do menu quando entrar na fase
                if (IsMusicStreamPlaying(menuMusic)) {
                    StopMusicStream(menuMusic);
                }
                DrawText("FASE CICLISMO", 300, 300, 40, YELLOW);
                break;

            case CREDITS:
                // Continuar música do menu nos créditos
                if (musicLoaded && menuMusic.frameCount > 0) {
                    if (!IsMusicStreamPlaying(menuMusic)) {
                        PlayMusicStream(menuMusic);
                    }
                    UpdateMusicStream(menuMusic);
                }
                DrawCredits();
                HandleInput();
                break;

            case GAME_OVER:
                DrawText("GAME OVER", 350, 350, 50, RED);
                break;
        }

        EndDrawing();
    }

    // Descarregar música antes de limpar
    if (musicLoaded) {
        StopMusicStream(menuMusic);
        UnloadMusicStream(menuMusic);
    }
    
    CleanupGame();
}

