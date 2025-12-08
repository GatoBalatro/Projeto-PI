#include <raylib.h>
#include "game.h"
#include <string.h>
#include <stdio.h>
#include "menu.h"


GameState currentState = MENU;

Music menuMusic;
bool musicLoaded = false;

void HandleInput();
void HandleInput();
void DrawMenu();
void LoadMenuMusic();

void HandleMenuInput() {
    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        
        Rectangle btnPlay = {450, 300, 300, 80};
        Rectangle btnCredits = {450, 400, 300, 80};
        Rectangle btnExit = {450, 500, 300, 80};
        Rectangle btnBack = {500, 550, 200, 60};
        
        if (CheckCollisionPointRec(mouse, btnPlay) || IsKeyPressed(KEY_ONE)) {
            currentState = FASE_CORRIDA;
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

void HandleInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentState == CREDITS || currentState == FASE_CORRIDA) {
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
