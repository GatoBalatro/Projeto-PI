#include <raylib.h>
#include "game.h" 
#include <string.h>

GameState currentState = MENU;
Player player;
Obstacle lixo;

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

    while (!WindowShouldClose()) {

        if(position_x_mais_longe < player.position.x) {
            position_x_mais_longe = player.position.x;
        }
        // Tratar input baseado no estado
        if (currentState == MENU) {
            HandleMenuInput();
            DrawMenu();
        }
        else if (currentState == CREDITS) {
            HandleMenuInput();
            DrawCredits();
        }
        else if (currentState == GAME) {
            
            if (!IsKeyDown(KEY_A)){
            if(IsKeyDown(KEY_D)) cameraOffset.x = cameraOffset.x + 1.0;
            
            cameraOffset.y = 350 - player.position.y;
            }
            timer += GetFrameTime();
            Rectangle lixoRec = { lixo.position.x, lixo.position.y, lixo.width, lixo.height };
            Rectangle playerRec = { player.position.x, player.position.y, lixo.width, lixo.height };
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
            DrawRectangleRec((Rectangle){player.position.x, player.position.y, player.width, player.height}, BLUE);
            DrawRectangleRec((Rectangle){lixo.position.x, lixo.position.y, lixo.width, lixo.height}, RED);
            DrawText("teste movendo bloquinho!", 190, 550, 20, BLACK);
            DrawText("ESC - Menu", 10, 650, 20, GRAY);

            if (CheckCollisionRecs(playerRec, lixoRec)) {
            DrawText("Colidiu com o lixo!", 450, 350, 40, RED);
            }   
            EndDrawing();
        }
    }

}

