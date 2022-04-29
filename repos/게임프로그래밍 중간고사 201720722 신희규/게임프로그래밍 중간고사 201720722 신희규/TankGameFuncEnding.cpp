#include "GameFunc.h"
#include "TankGameFuncEnding.h"
#include "TankGameFuncStage1.h"
#include <atlstr.h> // �ѱ��� ������ �ʿ���

Ending::Ending()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending.jpg");
	texture_ending_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(texture_ending_, NULL, NULL, &source_rectangle_ending_.w, &source_rectangle_ending_.h);
	destination_rectangle_ending_.x = 0;
	destination_rectangle_ending_.y = 0;
	source_rectangle_ending_.x = 0;
	source_rectangle_ending_.y = 0;
	destination_rectangle_ending_.w = 700;
	destination_rectangle_ending_.h = 700;

	g_font_end = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 64);
	SDL_Surface* tmp_surface_1 = TTF_RenderUTF8_Blended(g_font_end, CW2A(L"Game End", CP_UTF8), black);
	//�ؽ�Ʈ ��������
	g_game_end_text_kr_rect.x = 0;
	g_game_end_text_kr_rect.y = 0;
	g_game_end_text_kr_rect.w = tmp_surface_1->w;
	g_game_end_text_kr_rect.h = tmp_surface_1->h;

	g_game_end_text_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface_1);
	SDL_FreeSurface(tmp_surface_1);
}

void Ending::Update()
{
}


void Ending::Render()
{
	SDL_Rect tmp_r; // ȭ�鿡 ǥ�� �� ��ġ

	tmp_r.x = 200;
	tmp_r.y = 300;
	tmp_r.w = g_game_end_text_kr_rect.w;
	tmp_r.h = g_game_end_text_kr_rect.h;

	SDL_RenderCopy(g_renderer, texture_ending_, &source_rectangle_ending_, &destination_rectangle_ending_);

	SDL_RenderCopy(g_renderer, g_game_end_text_kr, &g_game_end_text_kr_rect, &tmp_r); // �ؽ�Ʈ ǥ��

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Ending::HandleEvents()
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
				g_current_game_phase = PHASE_INTRO;
			}
			break;

		default:
			break;
		}
	}
}


Ending::~Ending()
{
	SDL_DestroyTexture(g_game_end_text_kr); // ž���� �ؽ�Ʈ ����
	TTF_CloseFont(g_font_end); // ��Ʈ �޸� ����
	SDL_DestroyTexture(texture_ending_);
}