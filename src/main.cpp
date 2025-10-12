#include <raylib.h>
#include "game.h"  

int main() {
    InitWindow(800, 600, "Jogo Triatlo");
    SetTargetFPS(240);  
    RunGame();         // Chama o loop do jogo
    CloseWindow();
    return 0;
}