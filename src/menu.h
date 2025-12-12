#ifndef MENU_H
#define MENU_H

#include "game.h"
#include <raylib.h>

extern GameState currentState;
extern Music menuMusic;
extern bool musicLoaded;

void HandleMenuInput();
void HandleInput();
void DrawMenu();
void LoadMenuMusic();

#endif
