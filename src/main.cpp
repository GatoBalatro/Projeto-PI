#include <raylib.h>
#include "game.h"  

int main() {
    InitWindow(1200, 700, "Jogo Triatlo");
    InitAudioDevice();  // Inicializa o dispositivo de áudio
    SetTargetFPS(240);  
    RunGame();         
    CloseAudioDevice(); // Fecha o dispositivo de áudio
    CloseWindow();
    return 0;
}