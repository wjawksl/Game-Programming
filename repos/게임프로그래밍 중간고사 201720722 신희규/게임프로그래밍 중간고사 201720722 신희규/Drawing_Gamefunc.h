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
void DrawMissileCntText();
void DrawMissile();
void InitBGM();
void InitMissileChunk();
void InitScoreText();
void MakeGameObjTextures();
bool ObjectDistinct(SDL_Rect rect);

extern bool g_flag_running;
extern SDL_Renderer* g_renderer;