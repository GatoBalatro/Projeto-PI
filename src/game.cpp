#include <raylib.h>
#include "game.h" 
#include <string.h>
#include <stdio.h>  // Alternativa: usar stdio.h (versão C)

GameState currentState = MENU;
Player player;
Obstacle lixo;
Music menuMusic;  // Adicione esta linha
bool musicLoaded = false;  // Flag para controlar se a música já foi carregada

// Desenha o menu principal
void DrawMenu() {
    BeginDrawing();
    ClearBackground(DARKGREEN);
    
    // Título do jogo
    DrawText("JOGO TRIATLON", 400, 150, 60, YELLOW);
    DrawText("JOGO TRIATLON", 405, 155, 60, GOLD);
    
    // Botões do menu (áreas clicáveis)
    Rectangle btnPlay = {450, 300, 300, 80};
    Rectangle btnCredits = {450, 400, 300, 80};
    Rectangle btnExit = {450, 500, 300, 80};
    
    // Desenhar botões
    DrawRectangleRec(btnPlay, LIGHTGRAY);
    DrawRectangleRec(btnCredits, LIGHTGRAY);
    DrawRectangleRec(btnExit, LIGHTGRAY);
    
    // Bordas dos botões
    DrawRectangleLinesEx(btnPlay, 4, DARKGRAY);
    DrawRectangleLinesEx(btnCredits, 4, DARKGRAY);
    DrawRectangleLinesEx(btnExit, 4, DARKGRAY);
    
    // Textos dos botões
    DrawText("INICIAR JOGO", 510, 325, 32, DARKGRAY);
    DrawText("CRÉDITOS", 535, 425, 32, DARKGRAY);
    DrawText("SAIR", 565, 525, 32, DARKGRAY);
    
    if (musicLoaded) {
        char info[512];
        sprintf(info, "Musica: OK | FrameCount: %d | Tocando: %s | Tempo: %.1f/%.1f", 
                menuMusic.frameCount,
                IsMusicStreamPlaying(menuMusic) ? "SIM" : "NAO",
                GetMusicTimePlayed(menuMusic),
                GetMusicTimeLength(menuMusic));
        DrawText(info, 10, 50, 18, WHITE);
        
        if (IsAudioDeviceReady()) {
            DrawText("Audio Device: READY", 10, 70, 18, GREEN);
        } else {
            DrawText("Audio Device: NOT READY", 10, 70, 18, RED);
        }
    } else {
        DrawText("Musica: NAO CARREGADA", 10, 50, 20, RED);
    }
    
    EndDrawing();
}

// Desenha tela de créditos
void DrawCredits() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    DrawText("CRÉDITOS", 450, 100, 50, YELLOW);
    DrawText("DESENVOLVIDO POR:", 400, 200, 30, WHITE);
    
    // Créditos do grupo (ajuste conforme seu grupo)
    const char* credits[] = {
        "Integrante 0: <elso>",
        "Integrante 1: <phsm2>", 
        "Integrante 2: <vssb>",
        "Integrante 3: <ycms>"
    };
    
    int yOffset = 280;
    for (int i = 0; i < 5; i++) {
        DrawText(credits[i], 350, yOffset + (i * 40), 24, LIGHTGRAY);
    }
    
    // Botão voltar
    Rectangle btnBack = {500, 550, 200, 60};
    DrawRectangleRec(btnBack, RED);
    DrawRectangleLinesEx(btnBack, 3, WHITE);
    DrawText("VOLTAR", 550, 570, 24, WHITE);
    
    EndDrawing();
}

// Trata input do menu
void HandleMenuInput() {
    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        
        // Verificar clique nos botões
        Rectangle btnPlay = {450, 300, 300, 80};
        Rectangle btnCredits = {450, 400, 300, 80};
        Rectangle btnExit = {450, 500, 300, 80};
        Rectangle btnBack = {500, 550, 200, 60};
        
        if (CheckCollisionPointRec(mouse, btnPlay) || 
            (IsKeyPressed(KEY_ONE))) {
            currentState = GAME;
        }
        else if (CheckCollisionPointRec(mouse, btnCredits) || 
                 (IsKeyPressed(KEY_TWO))) {
            currentState = CREDITS;
        }
        else if (CheckCollisionPointRec(mouse, btnExit)) {
            CloseWindow();
        }
        else if (currentState == CREDITS && 
                 CheckCollisionPointRec(mouse, btnBack)) {
            currentState = MENU;
        }
    }
    
    // Teclas rápidas
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentState == CREDITS) currentState = MENU;
        else CloseWindow();
    }
}

// Função principal do jogo 
void RunGame() {

    player = { {400, 300}, {2.0f, 2.0f}, 50, 50 };
    lixo = { {1000, 200}, {2.0f, 2.0f}, 30, 30 };
    Vector2 cameraOffset = { 0, 0 };
    float position_x_mais_longe = 0.0f;
    float limite_tela = 1150.0f;

    float timer = 0.0f;

    // Carregar música do menu (apenas uma vez)
    if (!musicLoaded) {
        // Tentar múltiplos caminhos possíveis (OGG tem melhor suporte)
        const char* musicPaths[] = {
            "Dave-the-Diver-OST-On-the-boat.ogg",              // Pasta atual (build)
            "src/Dave-the-Diver-OST-On-the-boat.ogg",           // Pasta src
            "../src/Dave-the-Diver-OST-On-the-boat.ogg",       // Uma pasta acima
            "Dave the Diver OST - On the boat.ogg",            // Nome alternativo (se houver)
            "src/Dave the Diver OST - On the boat.ogg",        // Nome alternativo em src
            "Dave the Diver OST - On the boat.mp3",            // Fallback para MP3
            "src/Dave the Diver OST - On the boat.mp3"         // Fallback MP3 em src
        };
        
        bool loaded = false;
        for (int i = 0; i < 7; i++) {
            TraceLog(LOG_INFO, "Tentando carregar musica de: %s", musicPaths[i]);
            menuMusic = LoadMusicStream(musicPaths[i]);
            if (menuMusic.frameCount > 0) {
                SetMusicVolume(menuMusic, 0.7f);  // Volume 70%
                musicLoaded = true;
                loaded = true;
                TraceLog(LOG_INFO, "SUCCESS: Musica carregada de: %s", musicPaths[i]);
                TraceLog(LOG_INFO, "FrameCount: %d, SampleRate: %d", menuMusic.frameCount, menuMusic.stream.sampleRate);
                break;
            } else {
                TraceLog(LOG_WARNING, "Falha: %s (FrameCount: %d)", musicPaths[i], menuMusic.frameCount);
            }
        }
        
        if (!loaded) {
            TraceLog(LOG_ERROR, "ERRO: Nao foi possivel carregar a musica em nenhum caminho!");
            TraceLog(LOG_ERROR, "Verifique se o arquivo OGG esta na pasta src/ ou build/");
        }
    }

    while (!WindowShouldClose()) {

        if(position_x_mais_longe < player.position.x) {
            position_x_mais_longe = player.position.x;
        }
        // Tratar input baseado no estado
        if (currentState == MENU) {
            // Tocar música se foi carregada e não estiver tocando
            if (musicLoaded && menuMusic.frameCount > 0) {
                if (!IsMusicStreamPlaying(menuMusic)) {
                    PlayMusicStream(menuMusic);
                    TraceLog(LOG_INFO, "Musica iniciada");
                }
                UpdateMusicStream(menuMusic);  // Atualiza o stream de música
            } else if (!musicLoaded) {
                // Mostrar aviso na tela se música não foi carregada
                BeginDrawing();
                ClearBackground(DARKGREEN);
                DrawText("AVISO: Musica nao carregada!", 400, 600, 20, YELLOW);
                EndDrawing();
            }
            
            HandleMenuInput();
            DrawMenu();
        }
        else if (currentState == CREDITS) {
            // Continuar música nos créditos também
            if (musicLoaded && menuMusic.frameCount > 0) {
                if (!IsMusicStreamPlaying(menuMusic)) {
                    PlayMusicStream(menuMusic);
                }
                UpdateMusicStream(menuMusic);
            }
            
            HandleMenuInput();
            DrawCredits();
        }
        else if (currentState == GAME) {
            // Parar música quando entrar no jogo
            if (IsMusicStreamPlaying(menuMusic)) {
                StopMusicStream(menuMusic);
            }
            
            if (!IsKeyDown(KEY_A)){
            if(IsKeyDown(KEY_D)) cameraOffset.x = cameraOffset.x + 1.0;
            
            cameraOffset.y = 350 - player.position.y;
            }
            timer += GetFrameTime();
            Rectangle lixoRec = { lixo.position.x, lixo.position.y, (float)lixo.width, (float)lixo.height };
            Rectangle playerRec = { player.position.x, player.position.y, (float)player.width, (float)player.height };
            // movimento com WASD
        
            if (IsKeyDown(KEY_W)) player.position.y -= player.speed.y;
            if (IsKeyDown(KEY_S)) player.position.y += player.speed.y;

            if(!CheckCollisionRecs(playerRec, lixoRec)) {

            if (IsKeyDown(KEY_A)) player.position.x -= player.speed.x;
            if (IsKeyDown(KEY_D)) player.position.x += player.speed.x;
            lixo.position = {lixo.position.x - lixo.speed.x, lixo.position.y};
            }

            // Limites da tela 
            if (player.position.x <= 0) player.position.x = 0;
            if (player.position.x >= limite_tela) player.position.x = 1200 - player.width;
            if (player.position.y <= 0) player.position.y = 0;
            if (player.position.y + player.height >= 700) player.position.y = 700 - player.height;

            if (lixo.position.x + lixo.width <= 0) {
                lixo.position.x = 1200;
                lixo.position.y = 100 + lixo.position.y;
                if (lixo.position.y + lixo.height >= 650) {
                    lixo.position.y = 100;
                }
            }
  
                
            // Voltar ao menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentState = MENU;
                // Retomar música quando voltar ao menu
                if (!IsMusicStreamPlaying(menuMusic)) {
                    PlayMusicStream(menuMusic);
                }
            }

            BeginDrawing();
            ClearBackground(DARKGREEN);
      
            // Desenha o asfalto (retângulos)
            DrawRectangle(0, 100, 1200, 500, DARKGRAY);

            // Bordas da pista
            DrawRectangleLinesEx((Rectangle){-30, 100, 1500, 500}, 10, WHITE);

            // Faixa central tracejada
            for (int i = 30; i < 3600; i += 60) {
                DrawRectangle(i - cameraOffset.x,340, 40, 20, YELLOW);
            }

            // Curvas (se quiser)

            DrawText("Pista vista de cima", 10, 10, 30, WHITE);

            if (timer < 5.0f) DrawText("Fase 1", 500, 30 , 60, GOLD);
            
            DrawText("Use WASD pra mover o bloco!", 10, 10, 20, DARKGRAY);
            DrawRectangleRec((Rectangle){player.position.x, player.position.y, (float)player.width, (float)player.height}, BLUE);
            DrawRectangleRec((Rectangle){lixo.position.x, lixo.position.y, (float)lixo.width, (float)lixo.height}, RED);
            DrawText("teste movendo bloquinho!", 190, 550, 20, BLACK);
            DrawText("ESC - Menu", 10, 650, 20, GRAY);

            if (CheckCollisionRecs(playerRec, lixoRec)) {
            DrawText("Colidiu com o lixo!", 450, 350, 40, RED);
            }   
            EndDrawing();
        }
    }
    
    // Descarregar música ao sair
    if (musicLoaded) {
        UnloadMusicStream(menuMusic);
    }
}

