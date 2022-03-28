#include "Drawing_GameFunc.h"
#include <windows.h>
#include<iostream>
#include"SDL_image.h"

using namespace std;
class Missile { // 클래스 선언하고(미사일 객체) 구조체 배열로 만든 뒤 SDL_Texture만 해제하면 어케될까? 배열에서는 flag만 false로 바꿔주고
	/*bool flag = false;
	SDL_Rect destination_missile;
	SDL_Texture* missile_sheet_texture;*/
public :
	bool flag;
	SDL_Rect destination_missile;
	SDL_Texture* missile_sheet_texture;

	Missile() = default;
	Missile(bool f, SDL_Rect dm, SDL_Texture* mst) : flag(f), destination_missile(dm), missile_sheet_texture(mst) {}
};

Missile* missile_arr = new Missile[5]; // 클래스 배열 초기화

int g_input;
bool g_missile_flag;
bool g_flag_firing;

SDL_Texture* g_plane_sheet_texture; // 각 이미지들의 텍스쳐 선언
SDL_Texture* g_missile_sheet_texture;
SDL_Texture* g_bg_1_sheet_texture;

SDL_Rect g_bg_1_source_rect; // 배경 이미지에서 잘라오는 부분
SDL_Rect g_destination_bg_1;

SDL_Rect g_plane_source_rect; // 비행기 이미지에서 잘라오는 부분
SDL_Rect g_destination_plane;

SDL_Rect g_missile_source_rect; // 미사일 이미지에서 잘라오는 부분
SDL_Rect g_destination_missile;
// 흘러간 시간 기록
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void InitGame() {
	g_input = 0;
	
	g_flag_running = true;
	g_flag_firing = false;
	
	g_elapsed_time_ms = 0;

	// Clear the console screen.
	// 표준출력 화면을 깨끗히 지운다.

	SDL_Surface* bg_1_sheet_surface = IMG_Load("../../Resources/back1.jpg"); // 이미지 파일을 가져옴
	g_bg_1_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_1_sheet_surface);

	SDL_Surface* plane_sheet_surface= IMG_Load("../../Resources/Airplane.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(plane_sheet_surface, SDL_TRUE, SDL_MapRGB(plane_sheet_surface->format, 255, 255, 255));
	g_plane_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, plane_sheet_surface);

	SDL_Surface* missile_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);

	SDL_FreeSurface(plane_sheet_surface);

	g_bg_1_source_rect.x = 0; // 배경화면1 가져오기
	g_bg_1_source_rect.y = 0;
	g_bg_1_source_rect.w = 500;
	g_bg_1_source_rect.h = 600;

	g_destination_bg_1.x = 0; // 배경화면1 위치 설정
	g_destination_bg_1.y = 0;
	g_destination_bg_1.w = 600;
	g_destination_bg_1.h = 750;
	
	g_plane_source_rect.x = 50; // 비행기 잘라오기
	g_plane_source_rect.y = 305;
	g_plane_source_rect.w = 160;
	g_plane_source_rect.h = 160;

	g_destination_plane.x = 300; // 비행기 초기 위치
	g_destination_plane.y = 400;
	g_destination_plane.w = 75;
	g_destination_plane.h = 75;

	for (int i = 0; i < 5; i++)
	{
		missile_arr[i] = Missile(false, g_destination_plane, g_missile_sheet_texture);	
	}

	g_missile_source_rect.x = 308; // 미사일 잘라오기
	g_missile_source_rect.y = 401;
	g_missile_source_rect.w = 148;
	g_missile_source_rect.h = 91;

	system("cls");
}

/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Update() {

	for (int i = 0; i < 5; i++)
	{
		if (missile_arr[i].flag)
		{
			missile_arr[i].destination_missile.y -= 10;
			if (missile_arr[i].destination_missile.y < 0)
			{
				missile_arr[i].flag = false;
			}
		}
	}
	g_elapsed_time_ms += 33;

	if (g_input == 1) {		
		g_destination_plane.x -= 10;
	}
	else if (g_input == 2) {	
		g_destination_plane.x += 10;
	}
	else if (g_input == 3) {
		g_destination_plane.y -= 10;
	}
	else if (g_input == 4) {
		g_destination_plane.y += 10;
	}
	else if (g_input == 5) {
		//for문 돌려서 flag = false인것만 찾아낸 초기화 해주고 다음 Renderer에서 이미지를 그려준다.
		//g_missile_flag = true; // 얘도 배열이겠다 아마..
		//딜레이 주기
		if (g_flag_firing) return;
		g_flag_firing = true;

		for (int i = 0; i < 5; i++)
		{
			if (!missile_arr[i].flag)
			{
				missile_arr[i].flag = true;
				missile_arr[i].destination_missile = g_destination_plane;
				missile_arr[i].destination_missile.y -= 50; // 여기도 배열
				
				break;
			}
		}
		/*g_destination_missile = g_destination_plane;
		g_destination_missile.y = g_destination_plane.y - 50; // 여기도 배열*/
	}

	
}

/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {
	
	// Background 1
	SDL_RenderCopy(g_renderer, g_bg_1_sheet_texture, &g_bg_1_source_rect, &g_destination_bg_1);

	//g_plane_sheet_texture
	SDL_RenderCopy(g_renderer, g_plane_sheet_texture, &g_plane_source_rect, &g_destination_plane); // texture를 복사해서 화면에 나타내주는 함수
	
	//여기서도 for문 돌려서 flag가 true인 것만 그리기!
	for (int i = 0; i < 5; i++)
	{
		if (missile_arr[i].flag)
		{
			//g_missile_sheet_texture
			SDL_RenderCopy(g_renderer, missile_arr[i].missile_sheet_texture, &g_missile_source_rect, &missile_arr[i].destination_missile); // texture를 복사해서 화면에 나타내주는 함수
		}
	}
	//g_plane_sheet_texture
	//SDL_RenderCopy(g_renderer, g_missile_sheet_texture, &g_missile_source_rect, &g_destination_missile); // texture를 복사해서 화면에 나타내주는 함수

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
			else if (event.key.keysym.sym == SDLK_UP) {
				g_input = 3;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				g_input = 4;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {	
				g_input = 5;				
			}
			break;

		case SDL_KEYUP:
			if (g_flag_firing) g_flag_firing = false;
			g_input = 0;
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
	SDL_DestroyTexture(g_plane_sheet_texture); // 메모리 해제
	for (int i = 0; i < 5; i++)
	{
		SDL_DestroyTexture(missile_arr[i].missile_sheet_texture);
	}
}

