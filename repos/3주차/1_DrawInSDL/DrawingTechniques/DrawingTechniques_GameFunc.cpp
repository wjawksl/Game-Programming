#include "DrawingTechniques_GameFunc.h"
#include <windows.h>
#include"SDL_image.h"

int g_input;
// �귯�� �ð� ���
double g_elapsed_time_ms;

SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;


SDL_Rect g_source_rect;
SDL_Rect g_destination_rect;
SDL_Texture* g_ryu_sheet_texture;


/////////////////////////////////////////////////////////////
// InitGame() 
// ���α׷��� ���۵� �� ���ʿ� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���ӿ� �ʿ��� �ڿ�(�̹���, ���� ��)�� �ε��ϰ�, ���� �������� �ʱ�ȭ �ؾ��Ѵ�.
void InitGame() {
	g_input = 0;

	g_flag_running = true;
	g_elapsed_time_ms = 0;
	// BG
	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 1280;
	g_bg_source_rect.h = 720;

	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = g_bg_source_rect.w;
	g_bg_destination_rect.h = g_bg_source_rect.h;

	//BG
	SDL_Surface* bg_surface = IMG_Load("../../Resources/ddd.jpg"); // �̹��� ������ ������
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);

	SDL_FreeSurface(bg_surface);
	
	//Ryu Character
	SDL_Surface* ryu_sheet_surface = IMG_Load("../../Resources/60224.png"); // �̹��� ������ ������
	SDL_SetColorKey(ryu_sheet_surface, SDL_TRUE, SDL_MapRGB(ryu_sheet_surface->format, 0, 0, 248));
	g_ryu_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, ryu_sheet_surface);

	SDL_FreeSurface(ryu_sheet_surface);

	g_source_rect.x = 171;
	g_source_rect.y = 1647;
	g_source_rect.w = 67;
	g_source_rect.h = 140;

	g_destination_rect.x = 300;
	g_destination_rect.y = 200;
	g_destination_rect.w = g_source_rect.w;
	g_destination_rect.h = g_source_rect.h;


	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.

	system("cls");
}

/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Update() {
	
	g_elapsed_time_ms += 33;
}




/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {

	// Background
	//SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	//SDL_RenderClear(g_renderer); // ���� ĥ�ϱ�

	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);  // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�
	// Character
	/*SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(g_renderer, &g_char_pos);*/

	//g_ryu_sheet_texture
	for (int i = 0; i < 3; i++) {

		SDL_Rect r = g_destination_rect;

		r.x += i * 100;
		r.w *= (0.5f + i);
		r.h *= (0.5f + i);
		SDL_RenderCopy(g_renderer, g_ryu_sheet_texture, &g_source_rect, &r); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�
	}
	

	SDL_RenderPresent(g_renderer); // �鿡�� �׸� �׸��� �ѹ��� ������
}



/////////////////////////////////////////////////////////////
// HandleEvents() 
// �̺�Ʈ�� ó���ϴ� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				g_input = 2;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				g_input = 3;
			}
			break;

		case SDL_KEYUP:
			g_input = 0;
			break;

		case SDL_MOUSEBUTTONDOWN:

			// ���콺 ���� ��ư�� ���� ���� ��.
			if (event.button.button == SDL_BUTTON_LEFT) {
				g_input = 3;
			}
			break;

		default:
			break;
		}
	}
}




/////////////////////////////////////////////////////////////
// ClearGame() 
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
void ClearGame()
{
	SDL_DestroyTexture(g_ryu_sheet_texture);
}

