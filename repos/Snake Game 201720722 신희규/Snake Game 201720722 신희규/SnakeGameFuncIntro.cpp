#include "GameFunc.h"
#include "SnakeGameFuncIntro.h"
#include "SnakeGameFuncStage1.h"
#include <atlstr.h> // 한국어 쓰려면 필요함

Intro::Intro()
{
	/*g_intro_mus = Mix_LoadMUS("../../Resources/introBG.mp3"); // 배경음악 로드	
	Mix_VolumeMusic(60);

	if (g_current_game_phase == PHASE_INTRO)
		Mix_FadeInMusic(g_intro_mus, -1, 2000); // 배경음악 플레이*/

	// Button
	g_button_rect.x = 250;
	g_button_rect.y = 350;
	g_button_rect.w = 200;
	g_button_rect.h = 100;

	// For Texture
	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro.png");
	texture_intro_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(texture_intro_, NULL, NULL, &source_rectangle_intro_.w, &source_rectangle_intro_.h);
	destination_rectangle_intro_.x = source_rectangle_intro_.x = 0;
	destination_rectangle_intro_.y = source_rectangle_intro_.y = 0;
	destination_rectangle_intro_.w = 700;
	destination_rectangle_intro_.h = 700;

	g_font_intro = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 32);
	SDL_Surface* tmp_surface_1 = TTF_RenderUTF8_Blended(g_font_intro, CW2A(L"Game Start", CP_UTF8), black);
	//텍스트 가져오기
	g_game_start_text_kr_rect.x = 0;
	g_game_start_text_kr_rect.y = 0;
	g_game_start_text_kr_rect.w = tmp_surface_1->w;
	g_game_start_text_kr_rect.h = tmp_surface_1->h;

	g_game_start_text_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface_1);
	SDL_FreeSurface(tmp_surface_1);
}

void Intro::Update()
{
}


void Intro::Render()
{
	SDL_Rect tmp_r; // 화면에 표시 될 위치

	tmp_r.x = 265;
	tmp_r.y = 380;
	tmp_r.w = g_game_start_text_kr_rect.w;
	tmp_r.h = g_game_start_text_kr_rect.h;

	SDL_RenderCopy(g_renderer, texture_intro_, &source_rectangle_intro_, &destination_rectangle_intro_);

	SDL_SetRenderDrawColor(g_renderer, 127, 127, 127, 0);
	SDL_RenderFillRect(g_renderer, &g_button_rect);

	SDL_RenderCopy(g_renderer, g_game_start_text_kr, &g_game_start_text_kr_rect, &tmp_r); // 텍스트 표시

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Intro::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				// Get the cursor's x position.
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;

				if (mouse_x > g_button_rect.x &&
					mouse_y > g_button_rect.y &&
					mouse_x < g_button_rect.x + g_button_rect.w &&
					mouse_y < g_button_rect.y + g_button_rect.h
					)
				{
					
					//delete game_phases[g_current_game_phase];
					g_current_game_phase = PHASE_STAGE1;					
					//game_phases[PHASE_STAGE1] = new Stage1;
				}

				
			}
			break;

		default:
			break;
		}
	}
}

Intro::~Intro()
{
	SDL_DestroyTexture(g_game_start_text_kr); // 탑승중 텍스트 해제
	TTF_CloseFont(g_font_intro); // 폰트 메모리 해제
	SDL_DestroyTexture(texture_intro_);
	//Mix_FreeMusic(g_intro_mus);
}