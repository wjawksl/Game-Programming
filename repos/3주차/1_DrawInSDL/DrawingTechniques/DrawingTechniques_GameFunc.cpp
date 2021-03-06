#include "DrawingTechniques_GameFunc.h"
#include <windows.h>
#include"SDL_image.h"

int g_input;
// 흘러간 시간 기록
double g_elapsed_time_ms;

SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;

int g_oryugen_sprite_num;
int g_current_oryugen_id;

SDL_Rect g_ryu_source_rects[6];
SDL_Rect g_ryu_destination_rect;
SDL_Texture* g_ryu_sheet_texture;


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
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
	SDL_Surface* bg_surface = IMG_Load("../../Resources/ddd.jpg"); // 이미지 파일을 가져옴
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);

	SDL_FreeSurface(bg_surface);

	g_oryugen_sprite_num = 6;
	g_current_oryugen_id = 0;
	
	//Ryu Character
	SDL_Surface* ryu_sheet_surface = IMG_Load("../../Resources/60224.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(ryu_sheet_surface, SDL_TRUE, SDL_MapRGB(ryu_sheet_surface->format, 0, 0, 248));
	g_ryu_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, ryu_sheet_surface);

	SDL_SetTextureAlphaMod(g_ryu_sheet_texture, 170); // 투명도
	SDL_SetTextureColorMod(g_ryu_sheet_texture, 255, 0, 0); // 색 추가하기
	SDL_FreeSurface(ryu_sheet_surface);

	g_ryu_source_rects[0] = { 7,   1647, 69, 140 };
	g_ryu_source_rects[1] = { 94,  1647, 76, 140 };
	g_ryu_source_rects[2] = { 171, 1647, 68, 140 };
	g_ryu_source_rects[3] = { 240, 1647, 61, 140 };
	g_ryu_source_rects[4] = { 312, 1647, 54, 140 };
	g_ryu_source_rects[5] = { 390, 1647, 67, 140 };

	g_ryu_source_rects[1].x = 94;
	g_ryu_source_rects[1].y = 1647;
	g_ryu_source_rects[1].w = 76;
	g_ryu_source_rects[1].h = 140;

	g_ryu_destination_rect.x = 300;
	g_ryu_destination_rect.y = 200;
	g_ryu_destination_rect.w = g_ryu_source_rects[0].w;
	g_ryu_destination_rect.h = g_ryu_source_rects[0].h;


	// Clear the console screen.
	// 표준출력 화면을 깨끗히 지운다.

	system("cls");
}

/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Update() {
	
	g_current_oryugen_id++;
	if (g_current_oryugen_id >= g_oryugen_sprite_num) {
		g_current_oryugen_id = 0;
	}
	g_elapsed_time_ms += 33;
}




/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {

	// Background
	//SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	//SDL_RenderClear(g_renderer); // 배경색 칠하기

	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);  // texture를 복사해서 화면에 나타내주는 함수
	// Character
	/*SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(g_renderer, &g_char_pos);*/

	//g_ryu_sheet_texture
	for (int i = 0; i < 3; i++) {

		if (i == 0) 
		{
			SDL_SetTextureAlphaMod(g_ryu_sheet_texture, 170); // 투명도
			SDL_SetTextureColorMod(g_ryu_sheet_texture, 255, 0, 0); // 색 추가하기
		}
		else if (i == 1)
		{
			SDL_SetTextureAlphaMod(g_ryu_sheet_texture, 255); // 투명도
			SDL_SetTextureColorMod(g_ryu_sheet_texture, 0, 255, 255); // 색 추가하기
		}
		else 
		{
			SDL_SetTextureAlphaMod(g_ryu_sheet_texture, 100); // 투명도
			SDL_SetTextureColorMod(g_ryu_sheet_texture, 255, 255, 255); // 색 추가하기
		}

		SDL_Rect r = g_ryu_destination_rect;

		r.x += i * 100;
		r.w = g_ryu_source_rects[g_current_oryugen_id].w *(1.0f + i); // 점점 크게
		r.h = g_ryu_source_rects[g_current_oryugen_id].h *(1.0f + i); // 점점 크게
		SDL_RenderCopy(g_renderer,
			g_ryu_sheet_texture,
			&g_ryu_source_rects[g_current_oryugen_id],
			&r); // texture를 복사해서 화면에 나타내주는 함수
	}
	

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
	SDL_DestroyTexture(g_bg_texture);
}

