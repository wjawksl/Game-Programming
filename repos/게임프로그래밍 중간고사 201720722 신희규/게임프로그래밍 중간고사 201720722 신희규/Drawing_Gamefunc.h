#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();
void FireMissile();
void DrawGameText();
void DrawMissile();
void InitBGM();
void InitChunk();
void InitTexts();
void MakeGameObjTextures();
bool DistinctObject(SDL_Rect rect);
void PlayerMove();
void TankMoveAndFire();
void UpdateMissile();

extern bool g_flag_running;
extern SDL_Renderer* g_renderer;