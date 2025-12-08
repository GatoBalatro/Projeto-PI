#include <raylib.h>
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "menu.h"


void fase_natacao(){

    Texture2D playerSpriteSheet = {0}; 
    bool playerSpriteLoaded = false; 

    int playerFrameCount = 8; 
    float playerFrameWidth = 0.0f; 
    float animationTime = 0.0f; 
    float animationSpeed = 0.12f; // Nadar = um pouco mais lento 
    int currentFrame = 0; 

    Player player; 
    Obstacle tubarao; 
    Obstacle tubarao_2;
    Obstacle tubarao_3;
    Music waterMusic; 
    bool musicLoaded = false; 

    bool playerImune = false;
    float invencivelTimer = 0.0f;
    const float invencivelDuration = 1.0f; // 1 segundo imune

    player = { {300, 350}, {1.4f, 1.4f}, 50, 50 }; // Velocidade menor 
    tubarao = { {1200, 300}, {4.0f, -3.0f}, 40, 40 }; 
    tubarao_2 = { {500, player.position.y - 600}, {3.0f, -2.0f}, 40, 40 };
    tubarao_3 = { {-50, 300}, {5.0f, -4.0f}, 40, 40 };

    float timer = 0.0f; float limite_tela_x = 1050.0f; 
    
    // float corrente = 0.2f; // correnteza puxando jogador para a direita (opcional) 

    // --------------------------- // CARREGAR SPRITE SHEET // --------------------------- 

    if (!playerSpriteLoaded) { 

        const char* paths[] = { "swimmer.png", "src/swimmer.png", "assets/swimmer.png", "../src/swimmer.png" }; 

        for (int i = 0; i < 4; i++) { 

            playerSpriteSheet = LoadTexture(paths[i]); 

            if (playerSpriteSheet.id > 0) { 

                playerFrameWidth = playerSpriteSheet.width / playerFrameCount; 
                player.width = (int)playerFrameWidth; 
                player.height = playerSpriteSheet.height; 
                playerSpriteLoaded = true; 
                TraceLog(LOG_INFO, "Sprite do nadador: %s", paths[i]); 
                break; } 

            else { 

            UnloadTexture(playerSpriteSheet); 
            } 
        }

    } 
    // --------------------------- // CARREGAR MÚSICA DE ÁGUA // --------------------------- 
    if (!musicLoaded) { 

        const char* musicPaths[] = { "water.ogg", "src/water.ogg", "../src/water.ogg", "assets/water.ogg" }; 
        for (int i = 0; i < 4; i++) { 

        waterMusic = LoadMusicStream(musicPaths[i]); 

        if (waterMusic.frameCount > 0) { 

            SetMusicVolume(waterMusic, 0.7f); 
            PlayMusicStream(waterMusic); 
            musicLoaded = true; 
            break;  
            } 
        } 
    } 

    // --------------------------- // LOOP DA FASE DE NATAÇÃO // --------------------------- 


    
    while (!WindowShouldClose()) { // Atualizar música // 

        if (musicLoaded) UpdateMusicStream(waterMusic); 

        if (player.position.y < -10000) { 
        currentState = MENU; // Trocar para proxima fase
        break; 
        }

        timer += GetFrameTime(); // // Correnteza puxando o jogador horizontalmente // 
        // player.position.x += corrente; // Atualizar animação 
        bool moving = IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D); 

        if (moving) { 
            animationTime += GetFrameTime(); 

            if (animationTime >= animationSpeed) { 
            currentFrame = (currentFrame + 1) % playerFrameCount; animationTime = 0.0f; 
            } 
        } 

        else { 
            currentFrame = 0; 
        } 
        
        // Movimento com WASD 
        if (IsKeyDown(KEY_W)) player.position.y -= player.speed.y; 
        if (IsKeyDown(KEY_S)) player.position.y += player.speed.y; 
        if (IsKeyDown(KEY_A)) player.position.x -= player.speed.x; 
        if (IsKeyDown(KEY_D)) player.position.x += player.speed.x; 

        // Movimento da tubarao (obstáculo) 
        tubarao.position.x -= tubarao.speed.x; 
        tubarao_2.position.y -= tubarao_2.speed.y; 
        tubarao_3.position.x += tubarao_3.speed.x;


        if (tubarao.position.x + tubarao.width < 0) { 

            tubarao.position.x = 1200; 
            tubarao.position.y = player.position.y + GetRandomValue(-500, 100);
        } 

        if (tubarao_2.position.y  > player.position.y + 600) { 

            tubarao_2.position.x = 500 + GetRandomValue(-400,600); 
            tubarao_2.position.y = player.position.y - 600; 
        }

        if (tubarao_3.position.x - tubarao_3.width > 1200) { 

            tubarao_3.position.x = -50; 
            tubarao_3.position.y = player.position.y + GetRandomValue(-400, 200);
        }

        // Limites da água 
        if (player.position.x < 115) player.position.x = 115; 
        if (player.position.x > limite_tela_x) player.position.x = limite_tela_x; 
        if (player.position.y > 650 - player.height) player.position.y = 650 - player.height; 

        DrawText(TextFormat("Player Y: %.2f", player.position.y), 30, 80, 20, YELLOW); 

        Rectangle rPlayer = {player.position.x, player.position.y, player.width, player.height}; 
        Rectangle rtubarao = {tubarao.position.x, tubarao.position.y, tubarao.width, tubarao.height}; 
        Rectangle rtubarao_2 = {tubarao_2.position.x, tubarao_2.position.y, tubarao_2.width, tubarao_2.height}; 
        Rectangle rtubarao_3 = {tubarao_3.position.x, tubarao_3.position.y, tubarao_3.width, tubarao_3.height};

        bool colisao = CheckCollisionRecs(rPlayer, rtubarao); 
        colisao = colisao || CheckCollisionRecs(rPlayer, rtubarao_2); 
        colisao = colisao || CheckCollisionRecs(rPlayer, rtubarao_3);

        // --------------------------- // DESENHO // --------------------------- 

        BeginDrawing(); 
        
        // Apply camera (player stays near center) 
        BeginMode2D((Camera2D){ 
            .offset = {600, 350}, 
            .target = {600, player.position.y}, 
            .rotation = 0.0f, .zoom = 1.0f 
        }); // Desenhar boia 

        int chegadaY = -9950;
        DrawRectangle(100, chegadaY, 1000, 40, WHITE); // fundo branco

        // Padrão quadriculado preto e branco
        for (int i = 0; i < 20; i++) {
            Color c = (i % 2 == 0) ? BLACK : RED;
            DrawRectangle(100 + i * 50, chegadaY, 50, 40, c);
        }

        DrawText("CHEGADA", 520, chegadaY - 60, 40, YELLOW);


        ClearBackground((Color){ 0, 135, 255, 25}); // Azul escuro água Recife 
        DrawRectangle(0, -1000, 2000, 31000, (Color){0, 135, 255, 25}); // Ondinhas (efeito) 

        for (int i = 0; i < 1800; i += 60) { 
            DrawRectangle(100,-29000, 16,30000, ORANGE); 
            DrawRectangle(1100, -29000, 16, 30000, ORANGE); 
        } 
        // DrawText("TRIATHLON - FASE DA NATACAO", 300, 20, 30, WHITE); 

        if (timer < 5) DrawText("FASE 2 - NATAÇÃO", 450, 60, 40, YELLOW); // Desenhar jogador 

        if (playerSpriteLoaded) { 
            Rectangle frameRect = { currentFrame * playerFrameWidth, 0, playerFrameWidth, (float)playerSpriteSheet.height }; 
            DrawTextureRec(playerSpriteSheet, frameRect, (Vector2){player.position.x, player.position.y}, WHITE); 
        } 
        else { 
            DrawRectangle(player.position.x, player.position.y, player.width, player.height, BLACK); 
        } 

        
        DrawCircle(tubarao.position.x, tubarao.position.y, 20, RED); 
        DrawCircle(tubarao_2.position.x, tubarao_2.position.y, 20, RED); 
        DrawCircle(tubarao_3.position.x, tubarao_3.position.y, 20, RED);
        DrawText(TextFormat("Player Y: %.2f", player.position.y), player.position.x + 300, player.position.y, 20, YELLOW);



        if (colisao) DrawText("COLISAO!", 520, 350, 40, RED); 
        DrawText("WASD para nadar | ESC para sair", 20, 680, 20, WHITE); 


        EndDrawing(); 

    } // Limpeza // 

    if (musicLoaded) UnloadMusicStream(waterMusic); 
    if (playerSpriteLoaded) UnloadTexture(playerSpriteSheet); 

}