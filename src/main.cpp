#include <raylib.h>
#include "game.h"  

int main() {
    InitWindow(1200, 700, "Jogo Triatlo");
    SetTargetFPS(240);  
    RunGame();         
    CloseWindow();
    return 0;
}