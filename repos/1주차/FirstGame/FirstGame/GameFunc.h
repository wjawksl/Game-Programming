#pragma once

#include <iostream>
#include <string>
#include "SDL.h"

void InitGame();
void HandleEvents();
void Update();
void Render();
void ClearGame();

extern bool f_flag_running;
extern bool m_flag_running;