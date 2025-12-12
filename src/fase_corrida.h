#include <raylib.h>
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "fase_natacao.h"
#include "menu.h"

Player player;
Obstacle lixo;
Texture2D playerSpriteSheet = {0};
RenderTexture2D linhasTexture;
bool playerSpriteLoaded = false;

int playerFrameCount = 8;
float playerFrameWidth = 0.0f;
float animationTime = 0.0f;
const float animationSpeed = 0.15f;
int currentFrame = 0;
bool playerImune = false;
float invencivelTimer = 0.0f;
const float invencivelDuration = 1.0f; // 1 segundo imune


float scrollOffset = 0.0f;
int currentFase = 1;
bool faseComplete = false;

// Variáveis para SFX de colisão
Sound collisionSound = {0};
bool collisionSoundLoaded = false;
bool wasColliding = false;  // Flag para rastrear colisão anterior

// Variáveis para SFX de vitória
Sound victorySound = {0};
bool victorySoundLoaded = false;

void LoadPlayerSprite();
void LoadCollisionSound();
void LoadVictorySound();  // Adicione esta declaração
void UpdatePlayer();
void UpdateObstacles();
void UpdateAnimation(float dt);
void UpdateCamera(Vector2& cameraOffset, float& position_x_mais_longe);
void DrawWorld(const Vector2& cameraOffset);
void DrawPlayer();
void DrawUI(float timer);
void InitGame();
void UpdateGame();
void UpdateGame(float dt);
void DrawGame(Vector2& cameraOffset, float timer);
void CleanupGame();
void DrawCredits();
void ResetGame();
void UpdateScroll(float dt);

void InitGame() {
    ResetGame();
    player = { {400, 300}, {2.0f, 2.0f}, 50, 70 };
    lixo = { {1000, 200}, {1.20f, 1.20f}, 30, 30 };
    player.life = 3;


    LoadPlayerSprite();
    LoadCollisionSound();
    LoadVictorySound();  // Adicione esta linha
    wasColliding = false;  // Resetar flag de colisão
}

void LoadPlayerSprite() {
    if (playerSpriteLoaded) return;

    const char* paths[] = {
        "player.png", "src/player.png", "../src/player.png",
        "sprites/player.png", "assets/player.png"
    };

    for (int i = 0; i < 5; i++) {
        Texture2D tex = LoadTexture(paths[i]);
        if (tex.id > 0 && tex.width > 0) {
            playerSpriteSheet = tex;
            playerFrameWidth = (float)tex.width / playerFrameCount;
            player.width = (int)playerFrameWidth;
            player.height = tex.height;
            playerSpriteLoaded = true;
            TraceLog(LOG_INFO, "Player sprite loaded: %s", paths[i]);
            return;
        }
        UnloadTexture(tex);
    }
    TraceLog(LOG_WARNING, "Failed to load player sprite, using fallback rectangle");
}

void LoadCollisionSound() {
    if (collisionSoundLoaded) return;

    const char* soundPaths[] = {
        "cartoon_bite_sound_effect.mp3", "src/cartoon_bite_sound_effect.mp3", 
        "../src/cartoon_bite_sound_effect.mp3", "assets/cartoon_bite_sound_effect.mp3"
    };

    for (int i = 0; i < 4; i++) {
        collisionSound = LoadSound(soundPaths[i]);
        if (collisionSound.frameCount > 0) {
            SetSoundVolume(collisionSound, 0.7f);  // Volume 70%
            collisionSoundLoaded = true;
            TraceLog(LOG_INFO, "SFX de colisao carregado: %s", soundPaths[i]);
            return;
        }
    }
    TraceLog(LOG_WARNING, "Falha ao carregar SFX de colisao");
}

void LoadVictorySound() {
    if (victorySoundLoaded) return;

    const char* victoryPaths[] = {
        "victoryff.swf.ogg", "src/victoryff.swf.ogg", 
        "../src/victoryff.swf.ogg", "assets/victoryff.swf.ogg"
    };

    for (int i = 0; i < 4; i++) {
        victorySound = LoadSound(victoryPaths[i]);
        if (victorySound.frameCount > 0) {
            SetSoundVolume(victorySound, 0.8f);  // Volume 80%
            victorySoundLoaded = true;
            TraceLog(LOG_INFO, "SFX de vitoria carregado: %s", victoryPaths[i]);
            return;
        }
    }
    TraceLog(LOG_WARNING, "Falha ao carregar SFX de vitoria");
}




void UpdateGame(float dt) {
    // --- 1. Global Updates (Run regardless of state) ---

    // Atualizar invencibilidade
    if (playerImune) {
        invencivelTimer -= dt;
        if (invencivelTimer <= 0) {
            playerImune = false;
        }
    }


    if (currentState != FASE_CORRIDA) return;

    UpdateScroll(dt);

    if (faseComplete && IsKeyPressed(KEY_ENTER)) {
        currentFase++;
        ResetGame(); 
        TraceLog(LOG_INFO, "Iniciando Fase %d", currentFase);
    }

    UpdatePlayer();
    // UpdateObstacles();
    UpdateAnimation(dt);
}

void UpdatePlayer() {
    if (currentState != FASE_CORRIDA) return;

    // Vertical ALWAYS (W/S)
    if (IsKeyDown(KEY_W)) player.position.y -= player.speed.y;
    if (IsKeyDown(KEY_S)) player.position.y += player.speed.y;

    // Collision check (SCREEN coords - NO scrollOffset!)
    Rectangle playerRec = { player.position.x, player.position.y, (float)player.width, (float)player.height };
    Rectangle lixoRec = { lixo.position.x, lixo.position.y, (float)lixo.width, (float)lixo.height };

   if (lixo.position.x <= player.position.x - 600) {
        lixo.position.x = player.position.x + 800;
        lixo.position.y += 100;
        lixo.speed = {lixo.speed.x + 0.1f, lixo.speed.y + 0.1f}; // Aumenta a velocidade do lixo a cada reset
        if (lixo.position.y + lixo.height >= 650) {
            lixo.position.y = 100;
        }
    }
    
    bool colliding = CheckCollisionRecs(playerRec, lixoRec);

    // Tocar SFX de colisão quando detectar nova colisão
    if (colliding && !wasColliding && collisionSoundLoaded) {
        PlaySound(collisionSound);
    }
    wasColliding = colliding;  // Atualizar flag para próxima frame

    // ORIGINAL LOGIC: Horizontal A/D + lixo move ONLY if !colliding → "STOPS" on hit
    if (IsKeyDown(KEY_A)) player.position.x -= player.speed.x;
    if (IsKeyDown(KEY_D)) player.position.x += player.speed.x;
    lixo.position.x -= lixo.speed.x;

   
    if (colliding) {

        // Se ainda não está imune, perde vida
        if (!playerImune) {
            player.life -= 1;
            playerImune = true;
            invencivelTimer = invencivelDuration;

            TraceLog(LOG_WARNING, "Player colidiu! Vida restante: %d", player.life);
        }

        // Bloqueia movimento
        if (IsKeyDown(KEY_A)) player.position.x = player.position.x;
        if (IsKeyDown(KEY_D)) player.position.x = player.position.x;
    }


    // Bounds (full screen width)
    player.position.y = fmaxf(0, fminf(player.position.y, 700 - player.height));

      if (player.position.x > 30000) {
        // Tocar som de vitória ao completar a fase
        if (victorySoundLoaded && victorySound.frameCount > 0) {
            PlaySound(victorySound);
            TraceLog(LOG_INFO, "Som de vitoria tocado!");
        }
        currentState = FASE_NATACAO;
        TraceLog(LOG_INFO, "Fase %d completa!", currentFase);
    }
}




void UpdateAnimation(float dt) {
    bool isMoving = IsKeyDown(KEY_W) || IsKeyDown(KEY_S) ||
                    IsKeyDown(KEY_A) || IsKeyDown(KEY_D);

    if (isMoving) {
        animationTime += dt;
        if (animationTime >= animationSpeed) {
            currentFrame = (currentFrame + 1) % playerFrameCount;
            animationTime = 0.0f;
        }
    } else {
        currentFrame = 0;
        animationTime = 0.0f;
    }
}

void UpdateCamera(Vector2& cameraOffset, float& position_x_mais_longe) {
    if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) {
        cameraOffset.x += 1.0f;
        DrawText("Movendo para a direita", 10, 70, 20, WHITE);
    }
    // cameraOffset.y = 350 - player.position.y;
}

// ======================== RENDERING ========================
void DrawGame(Vector2& cameraOffset, float timer){  

    BeginDrawing();
    ClearBackground(DARKGREEN);

   if (currentState == FASE_CORRIDA) {
        // Apply camera (player stays near center)
        BeginMode2D((Camera2D){ 
            .offset = {600, 350}, 
            .target = {player.position.x, 350}, 
            .rotation = 0.0f, 
            .zoom = 1.0f 
        });

        DrawWorld(cameraOffset);  // Camera follows player
        DrawPlayer();
        DrawRectangleRec({
            lixo.position.x, lixo.position.y, 
            (float)lixo.width, (float)lixo.height
        }, RED);

        EndMode2D();

        DrawUI(timer);  // UI always on top (no camera)
    }

    EndDrawing();
}

void DrawWorld(const Vector2& cameraOffset) {

    // Tamanho da pista: 30000px de comprimento
    DrawRectangle(-200, 100, 30000, 500, DARKGRAY);
    DrawRectangleLinesEx({-200, 100, 30000, 500}, 10, WHITE);

    float finishX = 29800.0f;   // posição real no mundo

    // Faixa vertical branca (largura 40px)
    DrawRectangle(finishX, 100, 40, 500, WHITE);

    // Padrão xadrez
    for (int y = 100; y < 600; y += 40) {
        DrawRectangle(finishX,     y, 20, 20, BLACK);
        DrawRectangle(finishX+20,  y+20, 20, 20, BLACK);
    }

    DrawText("CHEGADA", finishX - 40, 70, 40, GOLD);
 

        DrawText("Pista vista de cima", 10, 10, 30, WHITE);
        DrawText(TextFormat("Fase: %d", currentFase), 1000, 10, 30, GOLD);
}

void DrawPlayer() {
    if (playerSpriteLoaded && playerSpriteSheet.id > 0) {
        Rectangle frameRect = {
            currentFrame * playerFrameWidth, 0,
            playerFrameWidth, (float)playerSpriteSheet.height
        };
        DrawTextureRec(playerSpriteSheet, frameRect,
                       player.position, WHITE);
    } else {
        DrawRectangleRec({player.position.x, player.position.y,
                         (float)player.width, (float)player.height}, BLUE);
    }
}

void DrawUI(float timer) {

    DrawText(TextFormat("Player X: %.2f", player.position.x), 10, 80, 20, YELLOW);

    if (timer < 5.0f) DrawText(TextFormat("Fase %d", currentFase), 500, 30, 60, GOLD);
    DrawText("Use W/S pra mover verticalmente!", 10, 40, 20, WHITE);  
    DrawText("ESC - Menu", 10, 650, 20, GRAY);

    Rectangle playerRec = {player.position.x, player.position.y, (float)player.width, (float)player.height};
    Rectangle lixoRec = {lixo.position.x - scrollOffset, lixo.position.y, (float)lixo.width, (float)lixo.height};

    // if (CheckCollisionRecs(playerRec, lixoRec)) {
    //     DrawText("Colidiu com o lixo! Fase reiniciada.", 350, 350, 30, RED);
    //     currentFase = 1;
    //     scrollOffset = 0.0f;
    // }
}


void CleanupGame() {
    if (playerSpriteLoaded) UnloadTexture(playerSpriteSheet);
    if (collisionSoundLoaded && collisionSound.frameCount > 0) UnloadSound(collisionSound);
    if (victorySoundLoaded && victorySound.frameCount > 0) UnloadSound(victorySound);
}

// ======================== FUNÇÕES DO MENU (NÃO REMOVA!) ========================




void DrawCredits() {
    // BeginDrawing();
    ClearBackground(BLACK);
    
    DrawText("CRÉDITOS", 450, 100, 50, YELLOW);
    DrawText("DESENVOLVIDO POR:", 400, 200, 30, WHITE);
    
    const char* credits[] = {
        "Integrante 0: <elso>",
        "Integrante 1: <phsm2>", 
        "Integrante 2: <vssb>",
        "Integrante 3: <ycms>"
    };
    
    for (int i = 0; i < 4; i++) {
        DrawText(credits[i], 350, 280 + (i * 40), 24, LIGHTGRAY);
    }
    
    Rectangle btnBack = {500, 550, 200, 60};
    DrawRectangleRec(btnBack, RED);
    DrawRectangleLinesEx(btnBack, 3, WHITE);
    DrawText("VOLTAR", 550, 570, 24, WHITE);
    
    // EndDrawing();
}



void ResetGame() {
    // player.position = {400, 300};
    // // player.speed = {2.0f, 2.0f};

    // lixo.position = {1000, 200};
    // // lixo.speed = {2.0f, 2.0f};
    // lixo.width = 30;
    // lixo.height = 30;
    
    // scrollOffset = 0.0f;
    // currentFase = 1;
    // faseComplete = false;
    // animationTime = 0.0f;
    // currentFrame = 0;
    wasColliding = false;  // Resetar flag de colisão ao reiniciar
}

void UpdateScroll(float dt) {
    if (currentState != FASE_CORRIDA || faseComplete) return;
    
    const float scrollSpeed = 100.0f;
    scrollOffset += scrollSpeed * dt;
    
    // Detect end of fase (dashed lines "end")
    if (scrollOffset >= 3550.0f && !faseComplete) {
        faseComplete = true;
        TraceLog(LOG_INFO, "Fase %d completa!", currentFase);
    }
}