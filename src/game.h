#ifndef GAME_H
#define GAME_H

// Estados do jogo
typedef enum {
    MENU = 0,
    FASE_CORRIDA,
    FASE_CICLISMO,
    FASE_NATACAO,
    GAME_OVER,
    CREDITS
} GameState;

// Estrutura do jogador
typedef struct {
    Vector2 position;
    Vector2 speed;
    int width;
    int height;
    int life;
} Player;

typedef struct {
    Vector2 position;
    Vector2 speed;
    int width;
    int height;
} NPC;


typedef struct {
    Vector2 position;
    Vector2 speed;
    int width;
    int height;

} Obstacle;

// Funções do jogo
void RunGame();
void DrawMenu();
void DrawCredits();
void HandleMenuInput();

#endif