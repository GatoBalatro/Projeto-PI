#include <raylib.h>
#include "game.h"  

int main() {
    InitWindow(1200, 700, "Menu do Jogo");
    SetTargetFPS(240);

    // Carregar imagem de fundo e fonte
    Texture2D background = LoadTexture("fundo.png");
    Texture2D startButton = LoadTexture("start.png");
    Texture2D creditsButton = LoadTexture("credits.png");

    // Posições dos botões
    Rectangle startBtn = { 300, 300, (float)startButton.width, (float)startButton.height };
    Rectangle creditsBtn = { 300, 400, (float)creditsButton.width, (float)creditsButton.height };

    // Estados
    bool showCredits = false;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        // Clique no botão Start
        if (CheckCollisionPointRec(mouse, startBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            DrawText("Iniciando o jogo...", 250, 550, 30, GREEN);
        }

        // Clique no botão Créditos
        if (CheckCollisionPointRec(mouse, creditsBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            showCredits = !showCredits;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha o fundo
        DrawTexture(background, 0, 0, WHITE);

        // Desenha botões
        DrawTexture(startButton, startBtn.x, startBtn.y, WHITE);
        DrawTexture(creditsButton, creditsBtn.x, creditsBtn.y, WHITE);

        if (showCredits) {
            DrawRectangle(150, 200, 500, 200, Fade(BLACK, 0.7f));
            DrawText("Feito por Pedro Balaka", 260, 270, 30, RAYWHITE);
            DrawText("Obrigado por jogar!", 280, 320, 25, RAYWHITE);
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(startButton);
    UnloadTexture(creditsButton);
    CloseWindow();

    return 0;
}