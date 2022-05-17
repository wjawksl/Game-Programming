#include "SnakeGameFuncStage1.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include "SnakeGameFuncEnding.h"
#include <atlstr.h> // �ѱ��� ������ �ʿ���

#define SIZE 8
using namespace std;

enum Key
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

 
// Init() ��� �����ڸ� �����
Stage1::Stage1()
{	
	MakeGameObjTextures();
	InitTexts();

	g_stage_flag_running = true;

	g_bg_source_rect.x = 0; // ���ȭ�� ��������
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 447;
	g_bg_source_rect.h = 446;

	g_destination_bg.x = 0; // ���ȭ�� ��ġ ����
	g_destination_bg.y = 0;
	g_destination_bg.w = 700;
	g_destination_bg.h = 700;

	g_snake_source_rect.x = 498; // �� ��������
	g_snake_source_rect.y = 75;
	g_snake_source_rect.w = 136;
	g_snake_source_rect.h = 135;

	g_destination_snake.x = 100; // �� ��ġ ����
	g_destination_snake.y = 100;
	g_destination_snake.w = 50;
	g_destination_snake.h = 50;

	g_apple_source_rect.x = 145; // ��� ��������
	g_apple_source_rect.y = 138;
	g_apple_source_rect.w = 392;
	g_apple_source_rect.h = 421;

	g_destination_apple.x = 500; // ��� ��ġ ����
	g_destination_apple.y = 500;
	g_destination_apple.w = 50;
	g_destination_apple.h = 50;

	MakeSnake(g_destination_snake);

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
	SnakeMove();
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Stage1::Render() {
	
	SDL_RenderCopy(g_renderer, g_bg_sheet_texture, &g_bg_source_rect, &g_destination_bg); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�

	for (auto iter = snakeList.begin(); iter != snakeList.end(); iter++)
	{
		SDL_RenderCopy(g_renderer, g_snake_sheet_texture, &g_snake_source_rect, &iter->destination_snake);
	}	

	SDL_RenderCopy(g_renderer, g_apple_sheet_texture, &g_apple_source_rect, &g_destination_apple); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�

	if(!g_stage_flag_running)
		DrawGameOverText();

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
			g_last_key = g_cur_key;
			if (event.key.keysym.sym == SDLK_LEFT) {				
				g_cur_key = Key::LEFT;				
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				g_cur_key = Key::RIGHT;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				g_cur_key = Key::UP;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				g_cur_key = Key::DOWN;
			}
			if (event.key.keysym.sym == SDLK_q)
			{
				Snake front = snakeList.front();

				if (g_cur_key == Key::LEFT) {
					front.destination_snake.x -= 50;
				}
				else if (g_cur_key == Key::RIGHT) {
					front.destination_snake.x += 50;
				}
				else if (g_cur_key == Key::UP) {
					front.destination_snake.y -= 50;
				}
				else if (g_cur_key == Key::DOWN) {
					front.destination_snake.y += 50;
				}

				MakeSnake(front);
			}
			if (event.key.keysym.sym == SDLK_SPACE) {				
				g_current_game_phase = PHASE_ENDING;				
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
	SDL_Surface* bg_sheet_surface = IMG_Load("../../Resources/BG.png"); // �̹��� ������ ������
	g_bg_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_sheet_surface);

	SDL_Surface* snake_sheet_surface = IMG_Load("../../Resources/snake.png"); // �̹��� ������ ������
	SDL_SetColorKey(snake_sheet_surface, SDL_TRUE, SDL_MapRGB(snake_sheet_surface->format, 255, 255, 255));
	g_snake_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, snake_sheet_surface);

	SDL_Surface* apple_sheet_surface = IMG_Load("../../Resources/apple.png"); // �̹��� ������ ������
	SDL_SetColorKey(apple_sheet_surface, SDL_TRUE, SDL_MapRGB(apple_sheet_surface->format, 255, 255, 255));
	g_apple_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, apple_sheet_surface);
	
	SDL_FreeSurface(bg_sheet_surface);
	SDL_FreeSurface(snake_sheet_surface);
	SDL_FreeSurface(apple_sheet_surface);
	
}

void Stage1::SnakeMove()
{	
	if (!g_stage_flag_running) return;

	
	Uint32 cur_time_ms = SDL_GetTicks();

	if (cur_time_ms - g_stage_last_time_ms < 150) return;
		
	Snake back = snakeList.back();
	
	SDL_Rect curRect = snakeList.front().destination_snake;
	back.destination_snake = curRect;

	if (g_cur_key == Key::LEFT) {
		back.destination_snake.x = curRect.x - 50;
	}
	else if (g_cur_key == Key::RIGHT) {
		back.destination_snake.x = curRect.x + 50;
	}
	else if (g_cur_key == Key::UP) {

		back.destination_snake.y = curRect.y - 50;
	}
	else if (g_cur_key == Key::DOWN) {
		back.destination_snake.y = curRect.y + 50;
	}
	if (g_cur_key != -1)
	{	
		CheckIsGameOver(back.destination_snake);
		if (!g_stage_flag_running) return;
		
		snakeList.push_front(back);
		snakeList.pop_back();
	}

	SDL_Delay(30);	// 30 �и������� ��ٸ���.
	g_stage_last_time_ms = cur_time_ms;
}

void Stage1::MakeSnake(Snake snake)
{
	Snake newSnake = Snake(snake.destination_snake);
	snakeList.push_front(snake);
}
void Stage1::CheckIsGameOver(SDL_Rect snakeHeadRect)
{
	
	if ((snakeHeadRect.x < 0 || 650 < snakeHeadRect.x) ||
		(snakeHeadRect.y < 0 || 650 < snakeHeadRect.y))
	{
		g_stage_flag_running = false; return;
	}
	for (auto iter = snakeList.begin(); iter != snakeList.end(); iter++)
	{
		if (iter == snakeList.begin()) continue;		

		if ((snakeHeadRect.x == iter->destination_snake.x) &&
			(snakeHeadRect.y == iter->destination_snake.y))
		{						
			g_stage_flag_running = false; break;
		}
	}	
}
void Stage1::CreateApple()
{

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

}
void Stage1::DrawGameOverText()
{
	SDL_Rect tmp_r; // ȭ�鿡 ǥ�� �� ��ġ

	tmp_r.x = 265;
	tmp_r.y = 325;
	tmp_r.w = g_gameover_text_kr_rect.w;
	tmp_r.h = g_gameover_text_kr_rect.h;

	SDL_RenderCopy(g_renderer, g_gameover_text_kr, &g_gameover_text_kr_rect, &tmp_r); // �ؽ�Ʈ ǥ��
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
	g_font_gameover = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 32);
	SDL_Surface* tmp_surface_1 = TTF_RenderUTF8_Blended(g_font_gameover, CW2A(L"Game Over!", CP_UTF8), black);
	//�ؽ�Ʈ ��������
	g_gameover_text_kr_rect.x = 0;
	g_gameover_text_kr_rect.y = 0;
	g_gameover_text_kr_rect.w = tmp_surface_1->w;
	g_gameover_text_kr_rect.h = tmp_surface_1->h;

	g_gameover_text_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface_1);
	SDL_FreeSurface(tmp_surface_1);
}
