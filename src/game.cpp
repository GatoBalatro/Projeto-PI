#include <raylib.h>
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

GameState currentState = MENU;
Player player;
Obstacle lixo;
Music menuMusic;
bool musicLoaded = false;
Texture2D playerSpriteSheet = {0};
bool playerSpriteLoaded = false;

int playerFrameCount = 8;
float playerFrameWidth = 0.0f;
float animationTime = 0.0f;
const float animationSpeed = 0.15f;
int currentFrame = 0;

// Forward declarations
void LoadPlayerSprite();
void LoadMenuMusic();
void UpdatePlayer();
void UpdateObstacles();
void UpdateAnimation(float dt);
void UpdateCamera(Vector2& cameraOffset, float& position_x_mais_longe);
void DrawWorld(const Vector2& cameraOffset);
void DrawPlayer();
void DrawUI(float timer);
void InitGame();
void HandleInput();
void UpdateGame();
void UpdateGame(float dt);
void DrawGame(float timer, const Vector2& cameraOffset, bool updateFarX);
void CleanupGame();
void DrawMenu();
void DrawCredits();
void HandleMenuInput();
// Main game loop (now clean and readable!)

void RunGame() {
    InitGame();  // Load resources once

    float timer = 0.0f;
    Vector2 cameraOffset = { 0, 0 };
    float position_x_mais_longe = 0.0f;
    const float limite_tela = 1150.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        timer += dt;

        HandleInput();           // Global input (ESC, menu navigation)
        UpdateGame(dt);          // Game logic (only when in GAME state)
        
        if (currentState == GAME) {
            UpdateCamera(cameraOffset, position_x_mais_longe);
        }

        DrawGame(timer, cameraOffset, position_x_mais_longe < player.position.x);
        
        position_x_mais_longe = fmaxf(position_x_mais_longe, player.position.x);
    }

    CleanupGame();  // Unload everything safely
}

void InitGame() {
    player = { {400, 300}, {2.0f, 2.0f}, 50, 50 };
    lixo = { {1000, 200}, {2.0f, 2.0f}, 30, 30 };

    LoadPlayerSprite();
    LoadMenuMusic();
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

void LoadMenuMusic() {
    if (musicLoaded) return;

    const char* paths[] = {
        "Dave-the-Diver-OST-On-the-boat.ogg",
        "src/Dave-the-Diver-OST-On-the-boat.ogg",
        "../src/Dave-the-Diver-OST-On-the-boat.ogg",
        "Dave the Diver OST - On the boat.ogg",
        "Dave the Diver OST - On the boat.mp3"
    };

    for (int i = 0; i < 5; i++) {
        Music music = LoadMusicStream(paths[i]);
        if (music.frameCount > 0) {
            menuMusic = music;
            SetMusicVolume(menuMusic, 0.7f);
            musicLoaded = true;
            TraceLog(LOG_INFO, "Menu music loaded: %s", paths[i]);
            return;
        }
    }
    TraceLog(LOG_ERROR, "Failed to load menu music from any path!");
}

void HandleInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentState == CREDITS || currentState == GAME) {
            currentState = MENU;
            if (musicLoaded) PlayMusicStream(menuMusic);
        } else {
            CloseWindow();
        }
    }

    if (currentState == MENU || currentState == CREDITS) {
        HandleMenuInput();
    }
}

void UpdateGame(float dt) {
    if (currentState != GAME) return;

    // Update music only in menu/credits
    if (musicLoaded) {
        if (currentState == MENU || currentState == CREDITS) {
            if (!IsMusicStreamPlaying(menuMusic)) PlayMusicStream(menuMusic);
            UpdateMusicStream(menuMusic);
        } else {
            StopMusicStream(menuMusic);
        }
    }

    UpdatePlayer();
    UpdateObstacles();
    UpdateAnimation(dt);
}

void UpdatePlayer() {
    if (currentState != GAME) return;

    if (IsKeyDown(KEY_W)) player.position.y -= player.speed.y;
    if (IsKeyDown(KEY_S)) player.position.y += player.speed.y;

    Rectangle playerRec = { player.position.x, player.position.y, (float)player.width, (float)player.height };
    Rectangle lixoRec = { lixo.position.x, lixo.position.y, (float)lixo.width, (float)lixo.height };

    bool colliding = CheckCollisionRecs(playerRec, lixoRec);

    if (!colliding) {
        if (IsKeyDown(KEY_A)) player.position.x -= player.speed.x;
        if (IsKeyDown(KEY_D)) player.position.x += player.speed.x;
        lixo.position.x -= lixo.speed.x;
    }

    // Screen bounds
    player.position.x = fmaxf(0, fminf(player.position.x, 1200 - player.width));
    player.position.y = fmaxf(0, fminf(player.position.y, 700 - player.height));
}

void UpdateObstacles() {
    if (lixo.position.x + lixo.width <= 0) {
        lixo.position.x = 1200;
        lixo.position.y += 100;
        if (lixo.position.y + lixo.height >= 650) {
            lixo.position.y = 100;
        }
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
    }
    cameraOffset.y = 350 - player.position.y;
}

// ======================== RENDERING ========================
void DrawGame(float timer, const Vector2& cameraOffset, bool updateFarX) {
    BeginDrawing();
    ClearBackground(DARKGREEN);

    if (currentState == MENU) {
        DrawMenu();
    }
    else if (currentState == CREDITS) {
        DrawCredits();
    }
    else if (currentState == GAME) {
        DrawWorld(cameraOffset);
        DrawPlayer();
        DrawRectangleRec({lixo.position.x, lixo.position.y, (float)lixo.width, (float)lixo.height}, RED);
        DrawUI(timer);
    }

    EndDrawing();
}

void DrawWorld(const Vector2& cameraOffset) {
    // Road
    DrawRectangle(0, 100, 1200, 500, DARKGRAY);
    DrawRectangleLinesEx({-30, 100, 1500, 500}, 10, WHITE);

    // Dashed center line
    for (int i = 30; i < 3600; i += 60) {
        DrawRectangle(i - cameraOffset.x, 340, 40, 20, YELLOW);
    }

    DrawText("Pista vista de cima", 10, 10, 30, WHITE);
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
    if (timer < 5.0f) DrawText("Fase 1", 500, 30, 60, GOLD);
    DrawText("Use WASD pra mover o bloco!", 10, 40, 20, WHITE);
    DrawText("ESC - Menu", 10, 650, 20, GRAY);

    Rectangle playerRec = {player.position.x, player.position.y, (float)player.width, (float)player.height};
    Rectangle lixoRec = {lixo.position.x, lixo.position.y, (float)lixo.width, (float)lixo.height};

    if (CheckCollisionRecs(playerRec, lixoRec)) {
        DrawText("Colidiu com o lixo!", 450, 350, 40, RED);
    }
}


void CleanupGame() {
    if (musicLoaded) UnloadMusicStream(menuMusic);
    if (playerSpriteLoaded) UnloadTexture(playerSpriteSheet);
}

// ======================== FUNÇÕES DO MENU (NÃO REMOVA!) ========================


void DrawMenu() {
    // BeginDrawing();
    ClearBackground(DARKGREEN);
    
    DrawText("JOGO TRIATLON", 400, 150, 60, YELLOW);
    DrawText("JOGO TRIATLON", 405, 155, 60, GOLD);
    
    Rectangle btnPlay = {450, 300, 300, 80};
    Rectangle btnCredits = {450, 400, 300, 80};
    Rectangle btnExit = {450, 500, 300, 80};
    
    DrawRectangleRec(btnPlay, LIGHTGRAY);
    DrawRectangleRec(btnCredits, LIGHTGRAY);
    DrawRectangleRec(btnExit, LIGHTGRAY);
    
    DrawRectangleLinesEx(btnPlay, 4, DARKGRAY);
    DrawRectangleLinesEx(btnCredits, 4, DARKGRAY);
    DrawRectangleLinesEx(btnExit, 4, DARKGRAY);
    
    DrawText("INICIAR JOGO", 510, 325, 32, DARKGRAY);
    DrawText("CRÉDITOS", 535, 425, 32, DARKGRAY);
    DrawText("SAIR", 565, 525, 32, DARKGRAY);

    // Debug da música
    if (musicLoaded) {
        char info[512];
        snprintf(info, sizeof(info), "Musica: OK | Tocando: %s | Tempo: %.1f/%.1f", 
                IsMusicStreamPlaying(menuMusic) ? "SIM" : "NAO",
                GetMusicTimePlayed(menuMusic),
                GetMusicTimeLength(menuMusic));
        DrawText(info, 10, 50, 18, WHITE);
    } else {
        DrawText("Musica: NAO CARREGADA", 10, 50, 20, RED);
    }
    
    // EndDrawing();
}

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

void HandleMenuInput() {
    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        
        Rectangle btnPlay = {450, 300, 300, 80};
        Rectangle btnCredits = {450, 400, 300, 80};
        Rectangle btnExit = {450, 500, 300, 80};
        Rectangle btnBack = {500, 550, 200, 60};
        
        if (CheckCollisionPointRec(mouse, btnPlay) || IsKeyPressed(KEY_ONE)) {
            currentState = GAME;
        }
        else if (CheckCollisionPointRec(mouse, btnCredits) || IsKeyPressed(KEY_TWO)) {
            currentState = CREDITS;
        }
        else if (CheckCollisionPointRec(mouse, btnExit)) {
            CloseWindow();
        }
        else if (currentState == CREDITS && CheckCollisionPointRec(mouse, btnBack)) {
            currentState = MENU;
        }
    }
}