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

    // Variáveis para sprite sheet do tubarão
    Texture2D tubaraoSpriteSheet = {0};
    bool tubaraoSpriteLoaded = false;
    int tubaraoFrameCount = 4;  // 4 frames no sprite sheet
    float tubaraoFrameWidth = 0.0f;
    float tubaraoAnimationTime = 0.0f;
    float tubaraoAnimationSpeed = 0.2f;  // Velocidade da animação do tubarão
    int tubaraoCurrentFrame = 0; 

    Player player; 
    Obstacle tubarao; 
    Obstacle tubarao_2;
    Obstacle tubarao_3;
    Music waterMusic; 
    bool musicLoaded = false; 
    
    // Variáveis para SFX de colisão
    Sound collisionSound = {0};
    bool collisionSoundLoaded = false;
    bool wasColliding = false;  // Flag para rastrear colisão anterior
    
    bool playerImune = false;
    float invencivelTimer = 0.0f;
    const float invencivelDuration = 1.0f; // 1 segundo imune
    
    player = { {300, 350}, {1.4f, 1.4f}, 50, 50}; // Velocidade menor 
    tubarao = { {1200, 300}, {4.0f, -3.0f}, 40, 40 }; 
    tubarao_2 = { {500, player.position.y - 600}, {3.0f, -2.0f}, 40, 40 };
    tubarao_3 = { {-50, 300}, {5.0f, -4.0f}, 40, 40 };

    player.life = 25;
    float livesTimer = 0.0f;
    
    float timer = 0.0f; float limite_tela_x = 1050.0f; 
    
    // float corrente = 0.2f; // correnteza puxando jogador para a direita (opcional) 

    // --------------------------- // CARREGAR SPRITE SHEET // --------------------------- 

    if (!playerSpriteLoaded) { 

        const char* paths[] = { 
        "../img/swimmer.png",  // Prioridade: busca na pasta irmã img
        "img/swimmer.png",     // Fallback: busca se executável estiver na raiz
        "swimmer.png"          // Fallback final
         }; 

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

    // --------------------------- // CARREGAR SPRITE SHEET DO TUBARÃO // --------------------------- 
    if (!tubaraoSpriteLoaded) {
        const char* tubaraoPaths[] = { 
            "../img/hai-fin-shadow-Sheet-Sheet.png", 
            "img/hai-fin-shadow-Sheet-Sheet.png", 
            "hai-fin-shadow-Sheet-Sheet.png"
        };

        for (int i = 0; i < 4; i++) {
            tubaraoSpriteSheet = LoadTexture(tubaraoPaths[i]);
            if (tubaraoSpriteSheet.id > 0 && tubaraoSpriteSheet.width > 0) {
                tubaraoFrameWidth = (float)tubaraoSpriteSheet.width / tubaraoFrameCount;
                // Ajustar tamanho dos tubarões baseado no sprite
                tubarao.width = (int)tubaraoFrameWidth;
                tubarao.height = tubaraoSpriteSheet.height;
                tubarao_2.width = (int)tubaraoFrameWidth;
                tubarao_2.height = tubaraoSpriteSheet.height;
                tubarao_3.width = (int)tubaraoFrameWidth;
                tubarao_3.height = tubaraoSpriteSheet.height;
                tubaraoSpriteLoaded = true;
                TraceLog(LOG_INFO, "Sprite do tubarao carregado: %s", tubaraoPaths[i]);
                break;
            } else {
                UnloadTexture(tubaraoSpriteSheet);
            }
        }
        if (!tubaraoSpriteLoaded) {
            TraceLog(LOG_WARNING, "Falha ao carregar sprite do tubarao, usando circulo vermelho");
        }
    }

    // --------------------------- // CARREGAR MÚSICA DE ÁGUA // --------------------------- 
    if (!musicLoaded) { 

        const char* musicPaths[] = {
            "../audio/water.ogg", 
            "audio/water.ogg", 
            "water.ogg"
         }; 
        for (int i = 0; i < 4; i++) { 

        waterMusic = LoadMusicStream(musicPaths[i]); 

        if (waterMusic.frameCount > 0) { 

            SetMusicVolume(waterMusic, 0.5f); 
            PlayMusicStream(waterMusic); 
            musicLoaded = true; 
            break;  
            } 
        } 
    } 

    // --------------------------- // CARREGAR SFX DE COLISÃO // --------------------------- 
    if (!collisionSoundLoaded) {
        const char* soundPaths[] = { 
                "../audio/cartoon_bite_sound_effect.mp3", 
                "audio/cartoon_bite_sound_effect.mp3", 
                "cartoon_bite_sound_effect.mp3"
        };
        
        for (int i = 0; i < 4; i++) {
            collisionSound = LoadSound(soundPaths[i]);
            if (collisionSound.frameCount > 0) {
                SetSoundVolume(collisionSound, 0.7f);  // Volume 70%
                collisionSoundLoaded = true;
                TraceLog(LOG_INFO, "SFX de colisao carregado: %s", soundPaths[i]);
                break;
            }
        }
        if (!collisionSoundLoaded) {
            TraceLog(LOG_WARNING, "Falha ao carregar SFX de colisao");
        }
    }

    // --------------------------- // LOOP DA FASE DE NATAÇÃO // --------------------------- 


    
    while (!WindowShouldClose()) { // Atualizar música // 

        float dt = GetFrameTime();
        timer += dt;

        if (musicLoaded) UpdateMusicStream(waterMusic); 

        if (player.position.y < -10000) { 
        currentState = MENU; // Trocar para proxima fase
        break; 
        }

        if (IsKeyPressed(KEY_ESCAPE) or player.life <= 0) {
            currentState = MENU;
            return;
        }

        timer += GetFrameTime(); // // Correnteza puxando o jogador horizontalmente // 
        // player.position.x += corrente; // Atualizar animação do jogador
        bool moving = IsKeyDown(KEY_W) || IsKeyDown(KEY_S) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D);

        // Atualizar animação do tubarão (sempre animando)
        tubaraoAnimationTime += GetFrameTime();
        if (tubaraoAnimationTime >= tubaraoAnimationSpeed) {
            tubaraoCurrentFrame = (tubaraoCurrentFrame + 1) % tubaraoFrameCount;
            tubaraoAnimationTime = 0.0f;
        } 

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
        if (IsKeyDown(KEY_S) && player.position.y > 0) player.position.y += player.speed.y;
         
        if (IsKeyDown(KEY_W)) player.position.y -= player.speed.y; 
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

        Rectangle rPlayer = {player.position.x, player.position.y, (float)player.width/1.5, (float)player.height/1.5}; 
        Rectangle rtubarao = {tubarao.position.x - 40, tubarao.position.y - 30,  (float)tubarao_2.height/3, (float)tubarao_2.width/8}; 
        Rectangle rtubarao_2 = {tubarao_2.position.x- 30, tubarao_2.position.y- 40, (float)tubarao.width/6, (float)tubarao.height/3}; 
        Rectangle rtubarao_3 = {tubarao_3.position.x - 40, tubarao_3.position.y - 30,(float)tubarao_3.height/3,  (float)tubarao_3.width/8};

        bool colisao = CheckCollisionRecs(rPlayer, rtubarao); 
        colisao = colisao || CheckCollisionRecs(rPlayer, rtubarao_2); 
        colisao = colisao || CheckCollisionRecs(rPlayer, rtubarao_3);

        // Tocar SFX de colisão quando detectar nova colisão
        if (colisao && !wasColliding && collisionSoundLoaded) {
            PlaySound(collisionSound);
        }
        wasColliding = colisao;  // Atualizar flag para próxima frame

        // Perder vida se colidir e não imune
        if (colisao && !playerImune) {
            player.life--;
            playerImune = true;
            invencivelTimer = invencivelDuration;
            livesTimer = 0.2f;
            TraceLog(LOG_INFO, "Vida perdida na natação! Vidas: %d", player.life);

            if (player.life <= 0) {
                currentState = MENU;
                TraceLog(LOG_INFO, "GAME OVER na natação - Voltando ao menu");
                break;
            }
        }
        // Atualizar timers
        if (playerImune) {
            invencivelTimer -= dt;
            if (invencivelTimer <= 0) playerImune = false;
        }
        if (livesTimer > 0) livesTimer -= dt;

        // --------------------------- // DESENHO // --------------------------- 

        BeginDrawing(); 
        
        // Apply camera (player stays near center) 
        BeginMode2D((Camera2D){ 
            .offset = {600, 350}, 
            .target = {600, player.position.y}, 
            .rotation = 0.0f, .zoom = 1.0f 
        }); // Desenhar boia 

        bool showLives = !playerImune || fmodf(livesTimer / 0.1f, 2.0f) < 1.0f;

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

        
        // Desenhar tubarões com sprite ou círculo vermelho como fallback
        if (tubaraoSpriteLoaded && tubaraoSpriteSheet.id > 0) {
            Rectangle tubaraoFrameRect = { 
                tubaraoCurrentFrame * tubaraoFrameWidth, 
                0.0f, 
                tubaraoFrameWidth, 
                (float)tubaraoSpriteSheet.height 
            };
            
            // Ponto de origem para rotação (centro do sprite)
            Vector2 origin = {tubaraoFrameWidth * 0.5f, tubaraoSpriteSheet.height * 0.5f};
            
            // Retângulo de destino (mesmo tamanho do frame)
            Rectangle destRect = {
                0.0f, 0.0f,
                tubaraoFrameWidth,
                (float)tubaraoSpriteSheet.height
            };
            
            // Desenhar tubarão 1 (move para ESQUERDA: x diminui)
            // Rotacionar 90 graus para apontar para esquerda
            destRect.x = tubarao.position.x;
            destRect.y = tubarao.position.y;
            DrawTexturePro(tubaraoSpriteSheet, tubaraoFrameRect, destRect, 
                          origin, 90.0f, WHITE);
            
            // Desenhar tubarão 2 (move para CIMA: y diminui)
            // Sem rotação (0 graus) - sprite já aponta para cima
            destRect.x = tubarao_2.position.x;
            destRect.y = tubarao_2.position.y;
            DrawTexturePro(tubaraoSpriteSheet, tubaraoFrameRect, destRect, 
                          origin, 0.0f, WHITE);
            
            // Desenhar tubarão 3 (move para DIREITA: x aumenta)
            // Rotacionar -90 graus para apontar para direita
            destRect.x = tubarao_3.position.x;
            destRect.y = tubarao_3.position.y;
            DrawTexturePro(tubaraoSpriteSheet, tubaraoFrameRect, destRect, 
                          origin, -90.0f, WHITE);
        } else {
            // Fallback: círculos vermelhos
            DrawCircle(tubarao.position.x, tubarao.position.y, 20, RED); 
            DrawCircle(tubarao_2.position.x, tubarao_2.position.y, 20, RED); 
            DrawCircle(tubarao_3.position.x, tubarao_3.position.y, 20, RED);
        }
        
        EndMode2D();  // Fechar modo 2D antes de desenhar textos na tela
        
        // DrawText(TextFormat("Player Y: %.2f", player.position.y), 30, 80, 20, YELLOW);
        DrawText(TextFormat("VIDAS: %d", player.life), 10, 110, 30, playerImune ? RED : YELLOW);
        if (colisao) DrawText("COLISAO!", 520, 350, 40, RED); 

        DrawText("WASD para nadar | ESC para reiniciar fase", 20, 680, 20, WHITE); 

        EndDrawing(); 

    } // Limpeza // 

    if (musicLoaded) UnloadMusicStream(waterMusic); 
    if (playerSpriteLoaded) UnloadTexture(playerSpriteSheet);
    if (tubaraoSpriteLoaded && tubaraoSpriteSheet.id > 0) UnloadTexture(tubaraoSpriteSheet);
    if (collisionSoundLoaded && collisionSound.frameCount > 0) UnloadSound(collisionSound);
}