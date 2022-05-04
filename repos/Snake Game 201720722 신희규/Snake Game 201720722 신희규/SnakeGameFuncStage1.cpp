#include "SnakeGameFuncStage1.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include "SnakeGameFuncEnding.h"
#include <atlstr.h> // �ѱ��� ������ �ʿ���

using namespace std;

#define SIZE 8

// Init() ��� �����ڸ� �����
Stage1::Stage1()
{	
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
void Stage1::Update() {

	g_elapsed_time_ms += 33;
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Stage1::Render() {
	
	// �鿡�� �׸� �׸��� �ѹ��� ������
	SDL_RenderPresent(g_renderer);
}

/////////////////////////////////////////////////////////////
// HandleEvents() 
// �̺�Ʈ�� ó���ϴ� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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
				g_key[4] = 0;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				/*delete game_phases[g_current_game_phase];
				g_current_game_phase = PHASE_ENDING;
				game_phases[g_current_game_phase] = new Ending;*/
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
// ClearGame() ��� �Ҹ��� ���
Stage1::~Stage1()
{
	//SDL_DestroyTexture(g_bg_sheet_texture); // ��� �޸� ����
	
	//TTF_CloseFont(g_font); // ��Ʈ �޸� ����

	//Mix_FreeChunk(g_missile_fire_sound);
}

void Stage1::MakeGameObjTextures()
{
	/*SDL_Surface* bg_sheet_surface = IMG_Load("../../Resources/BG.png"); // �̹��� ������ ������
	g_bg_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_sheet_surface);

	
	SDL_FreeSurface(bg_sheet_surface);*/
	
}

void Stage1::PlayerMove()
{
	/*if (g_key[0]) {
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
	}*/
}

bool Stage1::DistinctObject(SDL_Rect rect)
{
	/*int player_x = g_destination_charactor.x;
	int player_y = g_destination_charactor.y;

	int obj_rect_x_src = rect.x - rect.w / 2;
	int obj_rect_y_src = rect.y - rect.h / 2;

	int obj_rect_x_dst = rect.x + rect.w / 2;
	int obj_rect_y_dst = rect.y + rect.h / 2;

	if (player_x > obj_rect_x_src && player_y > obj_rect_y_src &&
		player_x < obj_rect_x_dst && player_y < obj_rect_y_dst) return true;

	return false;*/
}

void Stage1::DrawGameText()
{
	/*// Draw Score (Korean)
	SDL_Rect tmp_r; // ȭ�鿡 ǥ�� �� ��ġ

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
	SDL_FreeSurface(tmp_surface); // ��� �����������

	tmp_r.x = 650;
	tmp_r.y = 650;
	tmp_r.w = g_missile_cnt_rect.w;
	tmp_r.h = g_missile_cnt_rect.h;

	SDL_RenderCopy(g_renderer, g_missile_cnt_texture, &g_missile_cnt_rect, &tmp_r);
	SDL_DestroyTexture(g_missile_cnt_texture); // ��� �����������*/
}
void Stage1::InitChunk()
{
	/*g_missile_fire_sound = Mix_LoadWAV("../../Resources/fire.mp3"); // ȿ���� �ε�
	Mix_VolumeChunk(g_missile_fire_sound, 24);

	g_open_box_sound = Mix_LoadWAV("../../Resources/Coin.wav"); // ȿ���� �ε�
	Mix_VolumeChunk(g_open_box_sound, 24);

	g_ride_tank_sound = Mix_LoadWAV("../../Resources/IO.mp3"); // ȿ���� �ε�
	Mix_VolumeChunk(g_ride_tank_sound, 24);*/

}
void Stage1::InitTexts()
{
	/*// scoreText_kr Title
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28);
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(g_font, CW2A(L"ž����", CP_UTF8), black);
	//�ؽ�Ʈ ��������
	g_board_text_kr_rect.x = 0;
	g_board_text_kr_rect.y = 0;
	g_board_text_kr_rect.w = tmp_surface->w;
	g_board_text_kr_rect.h = tmp_surface->h;

	g_board_text_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);

	// scoreText_num
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28); // �������� ����*/
}
