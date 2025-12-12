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

        switch (currentState) {

            case MENU:
                ClearBackground(DARKGREEN);
                // Tocar música do menu se foi carregada
                if (musicLoaded && menuMusic.frameCount > 0) {
                    if (!IsMusicStreamPlaying(menuMusic)) {
                        PlayMusicStream(menuMusic);
                    }
                    UpdateMusicStream(menuMusic);  // Atualiza o stream de música
                }
                HandleInput();
                DrawMenu();      
                break;

            case FASE_CORRIDA:
                // Parar música do menu quando entrar na fase
                if (IsMusicStreamPlaying(menuMusic)) {
                    StopMusicStream(menuMusic);
                }
                HandleInput();  // Processar input (ESC para voltar ao menu)
                UpdateGame(dt);   // <-- atualiza player, obstáculos, animação, música, etc.
                DrawGame(cameraOffset, timer); 
                position_x_mais_longe = fmaxf(position_x_mais_longe, player.position.x);
                
                // Verificar transição de fase após renderização (evita frame perdido)
                // shouldTransitionToNatacao é definida em fase_corrida.h (já incluído)
                if (shouldTransitionToNatacao) {
                    shouldTransitionToNatacao = false;
                    currentState = FASE_NATACAO;
                    TraceLog(LOG_INFO, "Transição para FASE_NATACAO realizada");
                }
                break;

            case FASE_NATACAO:
                // Parar música do menu quando entrar na fase
                if (IsMusicStreamPlaying(menuMusic)) {
                    StopMusicStream(menuMusic);
                }
                fase_natacao();   // fase ainda WIP (tem seu próprio loop)
                break;

            case FASE_CICLISMO: // terceira fase acabou ficando de fora do projeto por termos ficado com apenas 3 membros no grupo 
                ClearBackground(BLACK);
                // Parar música do menu quando entrar na fase
                if (IsMusicStreamPlaying(menuMusic)) {
                    StopMusicStream(menuMusic);
                }
                DrawText("FASE CICLISMO", 300, 300, 40, YELLOW);
                break;

            case CREDITS:
                ClearBackground(BLACK);
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
                ClearBackground(BLACK);
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

