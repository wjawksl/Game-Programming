#include "SDL.h"
#include <iostream>
#include "GameFunc.h"
#include "GameFuncIntro.h"
#include "GameFuncStage1.h"
#include "GameFuncEnding.h"



/////////////////////////////////////////////////
// Declaration
SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

// Game Phases
int g_current_game_phase;


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("First Window", 100, 100, 1000, 700, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	
	InitGame();

	
	PhaseInterface* game_phases[3];
	game_phases[PHASE_INTRO] = new Intro;
	game_phases[PHASE_STAGE1] = new Stage1;
	game_phases[PHASE_ENDING] = new Ending;

	g_current_game_phase = PHASE_INTRO;

	g_last_time_ms = SDL_GetTicks();

	while ( g_flag_running )
	{
		Uint32 cur_time_ms = SDL_GetTicks();
		if ( cur_time_ms-g_last_time_ms < 33 )
			continue;

		game_phases[g_current_game_phase]->HandleEvents();
		game_phases[g_current_game_phase]->Update();
		game_phases[g_current_game_phase]->Render();

		g_last_time_ms = cur_time_ms;

	}

	ClearGame();

	for (int i = 0; i < 3; i++)
	{
		delete game_phases[i];
	}

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_CloseAudio();
	SDL_Quit();


	return 0;
}


