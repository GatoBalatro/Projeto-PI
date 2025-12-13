#include <raylib.h>
#include "game.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "fase_natacao.h"
#include "menu.h"


#define MAX_X_POSITION 7000;

Player player;
Obstacle lixo;
Texture2D playerSpriteSheet = {0};
RenderTexture2D linhasTexture;
bool playerSpriteLoaded = false;

float livesTimer = 0.0f;

int playerFrameCount = 8;
float playerFrameWidth = 0.0f;
float animationTime = 0.0f;
const float animationSpeed = 0.1f;
int currentFrame = 0;

// Variáveis para sprite do rato (obstáculo)
Texture2D ratoSpriteSheet = {0};
bool ratoSpriteLoaded = false;
int ratoFrameCount = 3;  // 3 frames no sprite sheet
float ratoFrameWidth = 0.0f;
float ratoAnimationTime = 0.0f;
const float ratoAnimationSpeed = 0.2f;  // Velocidade da animação do rato
int ratoCurrentFrame = 0;
bool playerImune = false;
float invencivelTimer = 0.0f;
const float invencivelDuration = 1.0f; // 1 segundo imune


float scrollOffset = 0.0f;
int currentFase = 1;
bool faseComplete = false;
bool shouldTransitionToNatacao = false;  // Flag para transição de fase após renderização

// Variáveis para SFX de colisão
Sound collisionSound = {0};
bool collisionSoundLoaded = false;
bool wasColliding = false;  // Flag para rastrear colisão anterior

// Variáveis para SFX de vitória
Sound victorySound = {0};
bool victorySoundLoaded = false;

// Variáveis para música da fase de corrida
Music gameMusic = {0};
bool gameMusicLoaded = false;


void LoadPlayerSprite();
void LoadRatoSprite();
void LoadCollisionSound();
void LoadVictorySound();
void LoadGameMusic();
void StopGameMusic();
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
    lixo = { {1000, 200}, {1.30f, 1.30f}, 30, 30 };
    player.life = 10;

    livesTimer = 0.0f;
    LoadPlayerSprite();
    LoadRatoSprite();
    LoadCollisionSound();
    LoadVictorySound();
    LoadGameMusic();
    wasColliding = false;  // Resetar flag de colisão
}

void LoadPlayerSprite() {
    if (playerSpriteLoaded) return;

    const char* paths[] = {
        "../img/player.png",
        "img/player.png",
        "player.png"
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

void LoadRatoSprite() {
    if (ratoSpriteLoaded) return;

    const char* paths[] = {
        "../img/rato-Sheet.png",
        "img/rato-Sheet.png",
        "rato-Sheet.png"
    };

    for (int i = 0; i < 5; i++) {
        Texture2D tex = LoadTexture(paths[i]);
        if (tex.id > 0 && tex.width > 0) {
            ratoSpriteSheet = tex;
            ratoFrameWidth = (float)tex.width / ratoFrameCount;
            lixo.width = (int)ratoFrameWidth;
            lixo.height = tex.height;
            ratoSpriteLoaded = true;
            TraceLog(LOG_INFO, "Rato sprite loaded: %s", paths[i]);
            return;
        }
        UnloadTexture(tex);
    }
    TraceLog(LOG_WARNING, "Failed to load rato sprite, using fallback rectangle");
}

void LoadCollisionSound() {
    if (collisionSoundLoaded) return;

    const char* soundPaths[] = {
        "../audio/punch.ogg",
        "audio/punch.ogg",
        "punch.ogg"
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

    const char* soundPaths[] = {
        "../audio/victoryff.swf.ogg",
        "audio/victoryff.swf.ogg",
        "victoryff.swf.ogg"
    };

    for (int i = 0; i < 4; i++) {
        victorySound = LoadSound(soundPaths[i]);
        if (victorySound.frameCount > 0) {
            SetSoundVolume(victorySound, 0.8f);  // Volume 80%
            victorySoundLoaded = true;
            TraceLog(LOG_INFO, "SFX de vitoria carregado: %s", soundPaths[i]);
            return;
        }
    }
    TraceLog(LOG_WARNING, "Falha ao carregar SFX de vitoria");
}

void LoadGameMusic() {
    if (gameMusicLoaded) return;

    const char* musicPaths[] = {
        "../audio/Pizza-Tower-OST-Thousand-March-_WAR_.ogg",
        "audio/Pizza-Tower-OST-Thousand-March-_WAR_.ogg",
        "Pizza-Tower-OST-Thousand-March-_WAR_.ogg"
    };

    for (int i = 0; i < 4; i++) {
        Music music = LoadMusicStream(musicPaths[i]);
        if (music.frameCount > 0) {
            gameMusic = music;
            SetMusicVolume(gameMusic, 0.7f);  // Volume 70%
            gameMusicLoaded = true;
            TraceLog(LOG_INFO, "Musica da fase de corrida carregada: %s", musicPaths[i]);
            return;
        }
    }
    TraceLog(LOG_WARNING, "Falha ao carregar musica da fase de corrida");
}

void StopGameMusic() {
    if (gameMusicLoaded && gameMusic.frameCount > 0) {
        if (IsMusicStreamPlaying(gameMusic)) {
            StopMusicStream(gameMusic);
            TraceLog(LOG_INFO, "Musica da fase de corrida parada");
        }
    }
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

    if (livesTimer > 0.0f) {
        livesTimer -= dt;
    }

    if (currentState != FASE_CORRIDA) return;

    // Atualizar música da fase
    if (gameMusicLoaded && gameMusic.frameCount > 0) {
        if (!IsMusicStreamPlaying(gameMusic)) {
            PlayMusicStream(gameMusic);
            TraceLog(LOG_INFO, "Musica da fase iniciada");
        }
        UpdateMusicStream(gameMusic);  // Atualiza o stream de música
    }

    // Atualizar animação do rato
    ratoAnimationTime += dt;
    if (ratoAnimationTime >= ratoAnimationSpeed) {
        ratoCurrentFrame = (ratoCurrentFrame + 1) % ratoFrameCount;
        ratoAnimationTime = 0.0f;
    }

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
    Rectangle playerRec = { player.position.x, player.position.y, (float)player.width/1.5, (float)player.height/1.5 };
    Rectangle lixoRec = { lixo.position.x + 100, lixo.position.y+50, (float)lixo.width/1.3, (float)lixo.height/2.4 };
    bool colliding = CheckCollisionRecs(playerRec, lixoRec);

   if (lixo.position.x <= player.position.x - 600) {
        lixo.position.x = player.position.x + 800;
        lixo.position.y += 100;
        lixo.speed = {lixo.speed.x + 0.11f, lixo.speed.y + 0.11f}; // Aumenta a velocidade do lixo a cada reset
        if (lixo.position.y + lixo.height >= 650) {
            lixo.position.y = 100;
        }
    }
    

    // Tocar SFX de colisão quando detectar nova colisão
    if (colliding && !wasColliding && collisionSoundLoaded) {
        PlaySound(collisionSound);
    }
    wasColliding = colliding;  // Atualizar flag para próxima frame

    if (IsKeyDown(KEY_A)) {
        if(player.position.x >0){ 
        player.position.x -= player.speed.x;
    }
    };

    if (IsKeyDown(KEY_D)) {
        player.position.x += player.speed.x;
    };
    lixo.position.x -= lixo.speed.x;

   
    if (colliding) {



            if (player.life <= 0) {
                currentState = MENU;
                TraceLog(LOG_INFO, "GAME OVER - Voltando ao menu");
                player.life = 10;
                playerImune = false;
                return;
            }

        
        // Se ainda não está imune, perde vida
        if (!playerImune) {
            livesTimer = 0.2f;  // Piscar vidas por 0.2s
            player.life -= 1;
            playerImune = true;
            invencivelTimer = invencivelDuration;

            TraceLog(LOG_WARNING, "Player colidiu! Vida restante: %d", player.life);
        }

        // Bloqueia movimento
        if (IsKeyDown(KEY_A)) player.position.x = player.position.x;
        if (IsKeyDown(KEY_D)) player.position.x = player.position.x;
    }


    // Bounds 
    player.position.y = fmaxf(0, fminf(player.position.y, 700 - player.height));
      int max_pos = MAX_X_POSITION;
      if (player.position.x > max_pos){
        // Tocar som de vitória ao completar a fase
        if (victorySoundLoaded && victorySound.frameCount > 0) {
            PlaySound(victorySound);
        }
        // Marcar para transição após renderização (evita frame perdido)
        shouldTransitionToNatacao = true;
        TraceLog(LOG_INFO, "Fase %d completa! Transição marcada.", currentFase);
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

void DrawMovingYellowStripes(Vector2 cameraOffset, int screenWidth, int screenHeight, int finishline) {
    const int roadY = 100;
    const int roadHeight = 500;
    
    const int stripeWidth = 140;
    const int stripeHeight = 28;
    const int gap = 200;                 // Distance between stripes
    const float parallaxFactor = 1.1f;   

    // Use camera offset as the scrolling source 
    float scroll = cameraOffset.x * parallaxFactor;
    float offset = fmodf(scroll, gap);
    if (offset < 0) offset += gap;
    int max_pos = MAX_X_POSITION;
    // Number of stripes needed to cover screen + buffer
    int stripesNeeded = (max_pos / gap) + 4;

    for (int i = -2; i < stripesNeeded; i++) {
        float worldX = i * gap;
        float screenX = worldX - offset;

        // Only draw stripes on the road
        if (screenX > -stripeWidth &&  screenX < finishline) {
            int y = roadY + roadHeight/2 - stripeHeight/2;

            DrawRectangle((int)screenX, y, stripeWidth, stripeHeight, YELLOW);
            DrawRectangleLinesEx((Rectangle){screenX, (float)y, (float)stripeWidth, (float)stripeHeight}, 3, BLACK);
        
        }
    }
}

void DrawGame(Vector2& cameraOffset, float timer){  

   if (currentState == FASE_CORRIDA) {
        ClearBackground(DARKGREEN);
        // Apply camera (player stays near center)
        BeginMode2D((Camera2D){ 
            .offset = {600, 350}, 
            .target = {player.position.x, 350}, 
            .rotation = 0.0f, 
            .zoom = 1.0f 
        });

        DrawWorld(cameraOffset);  // Camera follows player
        DrawPlayer();

        
        BeginMode2D((Camera2D){ 
            .offset = {600, 350}, 
            .target = {player.position.x, 350}, 
            .rotation = 0.0f, 
            .zoom = 1.0f 
        });

        
        DrawWorld(cameraOffset);
        
        DrawPlayer();
        // Desenhar rato com sprite ou retângulo vermelho como fallback
        if (ratoSpriteLoaded && ratoSpriteSheet.id > 0) {
            Rectangle ratoFrameRect = {
                ratoCurrentFrame * ratoFrameWidth, 0,
                ratoFrameWidth, (float)ratoSpriteSheet.height
            };
            // Espelhar horizontalmente
            Rectangle destRect = {
                lixo.position.x + ratoFrameWidth,  //  X Ajustado para compensar o espelhamento
                lixo.position.y,
                -ratoFrameWidth,  // Largura negativa espelha horizontalmente
                (float)ratoSpriteSheet.height
            };
            DrawTexturePro(ratoSpriteSheet, ratoFrameRect, destRect,
                          (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            // Fallback: retângulo vermelho
            DrawRectangleRec({
                lixo.position.x, lixo.position.y, 
                (float)lixo.width, (float)lixo.height
            }, RED);
        }

        EndMode2D();

        DrawUI(timer);  // UI sempre acima
    }
}

void DrawWorld(const Vector2& cameraOffset) {

    // Tamanho da pista: Max_x_position de comprimento
    int max_pos = MAX_X_POSITION;
    // Get dynamic screen dimensions
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float finishX = (float)max_pos;   // posição real no mundo
        

    DrawRectangle(-200, 100, max_pos + (screenWidth), 500, DARKGRAY);
    DrawRectangleLinesEx({-200, 100, (float)(max_pos + screenWidth), 500}, 10, WHITE);

    DrawMovingYellowStripes( cameraOffset, screenWidth, screenHeight, finishX);

    // Faixa vertical branca
    DrawRectangle(finishX, 100, 40, 500, WHITE);

    // Padrão xadrez
    for (int y = 100; y < 600; y += 40) {
        DrawRectangle(finishX,     y, 20, 20, BLACK);
        DrawRectangle(finishX+20,  y+20, 20, 20, BLACK);
    }

    DrawText("CHEGADA", finishX - 40, 70, 40, GOLD);
 

        // DrawText("Pista vista de cima", 10, 10, 30, WHITE);
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
    int max_pos = MAX_X_POSITION;
    // DrawText(TextFormat("distancia do final %.2f", ( max_pos / player.position.x) - 1), 10, 80, 20, YELLOW);


    if (timer < 5.0f) DrawText(TextFormat("Fase %d", currentFase), 500, 30, 60, GOLD);
    DrawText("Use W/A/S/D pra mover verticalmente!", 10, 40, 20, WHITE);  
    DrawText("ESC - Sair da partida", 10, 650, 20, GRAY);

    Rectangle playerRec = {player.position.x, player.position.y, (float)player.width, (float)player.height};
    Rectangle lixoRec = {lixo.position.x - scrollOffset, lixo.position.y, (float)lixo.width, (float)lixo.height};

    bool showLives = !playerImune || fmodf(livesTimer / 0.1f, 2.0f) < 1.0f;
    if (showLives) {
        DrawText(TextFormat("VIDAS: %d", player.life), 10, 110, 30, playerImune ? RED : YELLOW);
    }


    // if (CheckCollisionRecs(playerRec, lixoRec)) {
    //     DrawText("Colidiu com o lixo! Fase reiniciada.", 350, 350, 30, RED);
    //     currentFase = 1;
    //     scrollOffset = 0.0f;
    // }
}


void CleanupGame() {
    if (playerSpriteLoaded) UnloadTexture(playerSpriteSheet);
    if (ratoSpriteLoaded) UnloadTexture(ratoSpriteSheet);
    if (collisionSoundLoaded && collisionSound.frameCount > 0) UnloadSound(collisionSound);
    if (victorySoundLoaded && victorySound.frameCount > 0) UnloadSound(victorySound);
    if (gameMusicLoaded && gameMusic.frameCount > 0) {
        StopMusicStream(gameMusic);
        UnloadMusicStream(gameMusic);
    }
}

// ======================== FUNÇÕES DO MENU (NÃO REMOVER!) ========================




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
    player.position = {400, 300};
    // player.speed = {2.0f, 2.0f};

    lixo.position = {1000, 200};
    // lixo.speed = {2.0f, 2.0f};
    lixo.width = 30;
    lixo.height = 30;
    
    scrollOffset = 0.0f;
    currentFase = 1;
    faseComplete = false;
    animationTime = 0.0f;
    currentFrame = 0;
    player.life = 10;
    livesTimer = 0.0f;
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