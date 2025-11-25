#include <raylib.h>
#include "game.h"  

int main() {
    InitWindow(1200, 700, "Jogo Triatlo"); // abre a janela do jogo na resolução escolhida
    InitAudioDevice();  // Inicializa o dispositivo de áudio
    SetTargetFPS(240);  
    RunGame();         
    CloseAudioDevice(); // Fecha o dispositivo de áudio
    CloseWindow();  // Fecha a janela do jogo
    return 0;
}