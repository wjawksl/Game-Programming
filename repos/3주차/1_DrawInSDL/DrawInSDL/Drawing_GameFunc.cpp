#include "Drawing_GameFunc.h"
#include <windows.h>
#include"SDL_image.h"

int g_input;


SDL_Rect g_char_pos;
SDL_Texture* g_ryu_sheet_texture;

SDL_Rect g_source_rect;
SDL_Rect g_destination_rect;
// 흘러간 시간 기록
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void InitGame() {
	g_input = 0;
	
	g_flag_running = true;
	
	g_char_pos.x = 100;
	g_char_pos.y = 100;
	g_char_pos.w = 50;
	g_char_pos.h = 100;

	g_elapsed_time_ms = 0;

	// Clear the console screen.
	// 표준출력 화면을 깨끗히 지운다.

	SDL_Surface* ryu_sheet_surface= IMG_Load("../../Resources/60224.png"); // 이미지 파일을 가져옴
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

	system("cls");
}

/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Update() {
	if (g_input == 1) {
		g_char_pos.w = 50;
		g_char_pos.h = 200;
	}
	else if (g_input == 2) {
		g_char_pos.w = 200;
		g_char_pos.h = 100;
	}
	else if (g_input == 3) {
		g_char_pos.w = 10;
		g_char_pos.h = 10;
	}
	else {
		g_char_pos.w = 50;
		g_char_pos.h = 100;
	}

	g_char_pos.x += 10;

	if (g_char_pos.x > 800 - 50)
		g_char_pos.x = 0;

	g_elapsed_time_ms += 33;
}




/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {
	
	// Background
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	SDL_RenderClear(g_renderer); // 배경색 칠하기

	// Character
	SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(g_renderer, &g_char_pos);

	//g_ryu_sheet_texture
	SDL_RenderCopy(g_renderer, g_ryu_sheet_texture, &g_source_rect, &g_destination_rect); // texture를 복사해서 화면에 나타내주는 함수
	
	SDL_RenderPresent(g_renderer); // 백에서 그린 그림을 한번에 가져옴
}



/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
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

			// 마우스 왼쪽 버튼이 눌려 졌을 때.
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
// 프로그램이 끝날 때 한 번 호출되는 함수.
// 이 함수에서 사용된 자원(이미지, 사운드 등)과 메모리 등을 해제해야한다.
void ClearGame()
{
	SDL_DestroyTexture(g_ryu_sheet_texture);
}

