#include "TankGameFuncStage1.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include "TankGameFuncEnding.h"
#include <atlstr.h> // 한국어 쓰려면 필요함

using namespace std;

#define SIZE 8

// Init() 대신 생성자를 사용함
Stage1::Stage1()
{
	missile_arr = new Missile[SIZE]; // 클래스 배열 초기화

	g_missile_cnt = 0;
	g_flag_running = true;
	g_flag_firing = false;
	g_flag_interaction = false;
	g_flag_boarding = false;
	g_elapsed_time_ms = 0;
	g_tank_angle = 0;

	InitChunk(); //효과음 초기화

	InitTexts(); // 점수 초기화

	MakeGameObjTextures(); // 오브젝트들의 텍스트 가져오기

	g_bg_source_rect.x = 0; // 배경화면 가져오기
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 447;
	g_bg_source_rect.h = 446;

	g_destination_bg.x = 0; // 배경화면1 위치 설정
	g_destination_bg.y = 0;
	g_destination_bg.w = 700;
	g_destination_bg.h = 700;

	g_tank_source_rect.x = 206; // 탱크 잘라오기
	g_tank_source_rect.y = 0;
	g_tank_source_rect.w = 88;
	g_tank_source_rect.h = 102;

	g_destination_tank.x = 315; // 탱크 초기 위치
	g_destination_tank.y = 315;
	g_destination_tank.w = 75;
	g_destination_tank.h = 75;

	g_missile_source_rect.x = 349; // 미사일 잘라오기
	g_missile_source_rect.y = 1453;
	g_missile_source_rect.w = 69;
	g_missile_source_rect.h = 88;

	g_charactor_source_rect = { 182, 5, 356, 582 }; // 캐릭터 잘라오기
	g_destination_charactor = { 200, 200, 42, 66 }; // 캐릭터의 위치

	g_box_source_rect = { 233, 257, 1139, 885 }; // 보물상자 잘라오기
	g_destination_box = { 50, 100, 80, 64 }; // 보물상자의 위치

	for (int i = 0; i < SIZE; i++)
	{
		missile_arr[i] = Missile(false, -1, g_destination_tank, g_missile_sheet_texture);
	}
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
void Stage1::Update() {

	g_elapsed_time_ms += 33;

	UpdateMissile();

	if (!g_flag_boarding)
	{
		PlayerMove();
		if (g_key[4]) {

			if (g_flag_interaction) return;
			g_flag_interaction = true;

			if (DistinctObject(g_destination_tank)) // 탱크와 상호작용이라면
			{
				Mix_PlayChannel(-1, g_ride_tank_sound, 0);
				g_flag_boarding = true;
			}
			else if (DistinctObject(g_destination_box)) // 보물 상자와 상호작용이라면
			{
				Mix_PlayChannel(-1, g_open_box_sound, 0);
				g_missile_cnt += 5;
			}
		}
	}
	else
	{
		TankMoveAndFire();
		if (g_key[4])
		{
			if (g_flag_interaction) return;
			g_flag_interaction = true;

			Mix_PlayChannel(-1, g_ride_tank_sound, 0);
			g_flag_boarding = false;
		}
	}
}

/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Stage1::Render() {
	// 배경화면
	SDL_RenderCopy(g_renderer, g_bg_sheet_texture, &g_bg_source_rect, &g_destination_bg); // texture를 복사해서 화면에 나타내주는 함수

	// 탱크
	SDL_RenderCopyEx(g_renderer, g_tank_sheet_texture, &g_tank_source_rect,
		&g_destination_tank, 90 * g_tank_angle, NULL, SDL_FLIP_NONE);

	// 보물상자
	if (!g_missile_cnt)
		SDL_RenderCopy(g_renderer, g_box_sheet_texture, &g_box_source_rect, &g_destination_box);

	// 캐릭터
	if (!g_flag_boarding)
		SDL_RenderCopy(g_renderer, g_charactor_sheet_texture, &g_charactor_source_rect, &g_destination_charactor);

	//여기서도 for문 돌려서 flag가 true인 것만 그리기!
	DrawMissile();

	// 텍스트
	DrawGameText();

	// 백에서 그린 그림을 한번에 가져옴
	SDL_RenderPresent(g_renderer);
}

/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Stage1::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT) {
				g_key[0] = 1;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				g_key[1] = 1;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				g_key[2] = 1;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				g_key[3] = 1;
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				g_key[4] = 1;
			}
			break;

		case SDL_KEYUP:
			if (g_flag_boarding && g_flag_firing) g_flag_firing = false;

			if (event.key.keysym.sym == SDLK_LEFT) {
				g_key[0] = 0;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				g_key[1] = 0;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				g_key[2] = 0;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				g_key[3] = 0;
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				if (g_flag_interaction) g_flag_interaction = false;
				g_key[4] = 0;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				delete game_phases[g_current_game_phase];
				g_current_game_phase = PHASE_ENDING;
				game_phases[g_current_game_phase] = new Ending;
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
// ClearGame() 대신 소멸자 사용
Stage1::~Stage1()
{
	SDL_DestroyTexture(g_bg_sheet_texture); // 배경 메모리 해제
	SDL_DestroyTexture(g_tank_sheet_texture); // 탱크 메모리 해제
	SDL_DestroyTexture(g_box_sheet_texture); // 보물 상자 메모리 해제
	SDL_DestroyTexture(g_charactor_sheet_texture); // 캐릭터 메모리 해제
	SDL_DestroyTexture(g_missile_cnt_texture); // 잔여 미사일 수 텍스쳐 메모리 해제
	SDL_DestroyTexture(g_board_text_kr); // 탑승중 텍스트 해제
	SDL_DestroyTexture(g_missile_cnt_texture);

	TTF_CloseFont(g_font); // 폰트 메모리 해제

	for (int i = 0; i < SIZE; i++)
	{
		SDL_DestroyTexture(missile_arr[i].missile_sheet_texture); // 미사일 메모리 해제
		missile_arr[i].~Missile();
	}

	Mix_FreeChunk(g_missile_fire_sound);
	Mix_FreeChunk(g_open_box_sound);
	Mix_FreeChunk(g_ride_tank_sound);
}

void Stage1::MakeGameObjTextures()
{
	SDL_Surface* bg_sheet_surface = IMG_Load("../../Resources/BG.png"); // 이미지 파일을 가져옴
	g_bg_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_sheet_surface);

	SDL_Surface* tank_sheet_surface = IMG_Load("../../Resources/Tank.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(tank_sheet_surface, SDL_TRUE, SDL_MapRGB(tank_sheet_surface->format, 255, 255, 255));
	g_tank_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, tank_sheet_surface);

	SDL_Surface* missile_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);

	SDL_Surface* charactor_sheet_surface = IMG_Load("../../Resources/charactor.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(charactor_sheet_surface, SDL_TRUE, SDL_MapRGB(charactor_sheet_surface->format, 255, 255, 255));
	g_charactor_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, charactor_sheet_surface);

	SDL_Surface* box_sheet_surface = IMG_Load("../../Resources/box.jpg"); // 이미지 파일을 가져옴
	SDL_SetColorKey(box_sheet_surface, SDL_TRUE, SDL_MapRGB(box_sheet_surface->format, 255, 255, 255));
	g_box_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, box_sheet_surface);

	SDL_FreeSurface(charactor_sheet_surface);
	SDL_FreeSurface(box_sheet_surface);
	SDL_FreeSurface(bg_sheet_surface);
	SDL_FreeSurface(tank_sheet_surface);
	SDL_FreeSurface(missile_sheet_surface);
}

void Stage1::DrawMissile()
{
	for (int i = 0; i < SIZE; i++)
	{
		if (missile_arr[i].flag)
		{
			//g_missile_sheet_texture
			SDL_RenderCopyEx(g_renderer, missile_arr[i].missile_sheet_texture,
				&g_missile_source_rect, &missile_arr[i].destination_missile, 90 * (double)missile_arr[i].angle, NULL, SDL_FLIP_NONE); // texture를 복사해서 화면에 나타내주는 함수
		}
	}
}
void Stage1::PlayerMove()
{
	if (g_key[0]) {
		g_destination_charactor.x -= 10;
	}
	if (g_key[1]) {
		g_destination_charactor.x += 10;
	}
	if (g_key[2]) {
		g_destination_charactor.y -= 10;
	}
	if (g_key[3]) {
		g_destination_charactor.y += 10;
	}
}
void Stage1::TankMoveAndFire()
{
	if (g_key[0]) {
		g_tank_angle = 3;

		if (g_missile_cnt > 0)
			FireMissile();
	}
	if (g_key[1]) {
		g_tank_angle = 1;

		if (g_missile_cnt > 0)
			FireMissile();
	}
	if (g_key[2]) {
		g_tank_angle = 0;

		if (g_missile_cnt > 0)
			FireMissile();
	}
	if (g_key[3]) {
		g_tank_angle = 2;

		if (g_missile_cnt > 0)
			FireMissile();
	}

}
bool Stage1::DistinctObject(SDL_Rect rect)
{
	int player_x = g_destination_charactor.x;
	int player_y = g_destination_charactor.y;

	int obj_rect_x_src = rect.x - rect.w / 2;
	int obj_rect_y_src = rect.y - rect.h / 2;

	int obj_rect_x_dst = rect.x + rect.w / 2;
	int obj_rect_y_dst = rect.y + rect.h / 2;

	if (player_x > obj_rect_x_src && player_y > obj_rect_y_src &&
		player_x < obj_rect_x_dst && player_y < obj_rect_y_dst) return true;

	return false;
}
void Stage1::FireMissile()
{
	if (g_flag_firing) return;
	g_flag_firing = true;

	for (int i = 0; i < SIZE; i++)
	{
		if (!missile_arr[i].flag)
		{
			missile_arr[i].flag = true;
			missile_arr[i].destination_missile.x = g_destination_tank.x + 75 * dx[(int)g_tank_angle];
			missile_arr[i].destination_missile.y = g_destination_tank.y + 75 * dy[(int)g_tank_angle]; // 미사일 위치 설정
			missile_arr[i].destination_missile.w = 56;
			missile_arr[i].destination_missile.h = 72;
			missile_arr[i].angle = (int)g_tank_angle;

			Mix_PlayChannel(-1, g_missile_fire_sound, 0); //발사 효과음
			g_missile_cnt--;
			break;
		}
	}
}

void Stage1::UpdateMissile()
{
	for (int i = 0; i < SIZE; i++) // 미사일의 움직임 관리
	{
		if (missile_arr[i].flag)
		{
			missile_arr[i].destination_missile.x += 15 * dx[missile_arr[i].angle];
			missile_arr[i].destination_missile.y += 15 * dy[missile_arr[i].angle];

			if (missile_arr[i].destination_missile.x < 0 || 650 < missile_arr[i].destination_missile.x ||
				missile_arr[i].destination_missile.y < 0 || 650 < missile_arr[i].destination_missile.y)
			{
				missile_arr[i].flag = false;
				missile_arr[i].angle = -1;
			}
		}
	}
}

void Stage1::DrawGameText()
{
	// Draw Score (Korean)
	SDL_Rect tmp_r; // 화면에 표시 될 위치

	if (g_flag_boarding)
	{
		tmp_r.x = 100;
		tmp_r.y = 650;
		tmp_r.w = g_board_text_kr_rect.w;
		tmp_r.h = g_board_text_kr_rect.h;
		SDL_RenderCopy(g_renderer, g_board_text_kr, &g_board_text_kr_rect, &tmp_r);
	}

	// Draw Score (Number)
	string missile_cnt_str = to_string(g_missile_cnt);
	const char* cstr = missile_cnt_str.c_str();
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(g_font, cstr, black);

	g_missile_cnt_rect.x = 0;
	g_missile_cnt_rect.y = 0;
	g_missile_cnt_rect.w = tmp_surface->w;
	g_missile_cnt_rect.h = tmp_surface->h;

	g_missile_cnt_texture = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface); // 계속 해제해줘야함

	tmp_r.x = 650;
	tmp_r.y = 650;
	tmp_r.w = g_missile_cnt_rect.w;
	tmp_r.h = g_missile_cnt_rect.h;

	SDL_RenderCopy(g_renderer, g_missile_cnt_texture, &g_missile_cnt_rect, &tmp_r);
	SDL_DestroyTexture(g_missile_cnt_texture); // 계속 해제해줘야함
}
void Stage1::InitChunk()
{
	g_missile_fire_sound = Mix_LoadWAV("../../Resources/fire.mp3"); // 효과음 로드
	Mix_VolumeChunk(g_missile_fire_sound, 24);

	g_open_box_sound = Mix_LoadWAV("../../Resources/Coin.wav"); // 효과음 로드
	Mix_VolumeChunk(g_open_box_sound, 24);

	g_ride_tank_sound = Mix_LoadWAV("../../Resources/IO.mp3"); // 효과음 로드
	Mix_VolumeChunk(g_ride_tank_sound, 24);

}
void Stage1::InitTexts()
{
	// scoreText_kr Title
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28);
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(g_font, CW2A(L"탑승중", CP_UTF8), black);
	//텍스트 가져오기
	g_board_text_kr_rect.x = 0;
	g_board_text_kr_rect.y = 0;
	g_board_text_kr_rect.w = tmp_surface->w;
	g_board_text_kr_rect.h = tmp_surface->h;

	g_board_text_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);

	// scoreText_num
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28); // 전역변수 선언
}
