#include "SnakeGameFuncStage1.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include "SnakeGameFuncEnding.h"
#include <atlstr.h> // �ѱ��� ������ �ʿ���

#define SIZE 8
#define CELL 50
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
	
	g_bg_1_source_rect.x = 0;
	g_bg_1_source_rect.y = 0;
	g_bg_1_source_rect.w = 700;
	g_bg_1_source_rect.h = 700;

	g_bg_2_source_rect.x = 50;
	g_bg_2_source_rect.y = 50;
	g_bg_2_source_rect.w = 600;
	g_bg_2_source_rect.h = 600;
	
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

	pair<int, int> tempPos = CreateRandomPosition(); // ����� ��ġ ����
	g_destination_apple.x = tempPos.first;
	g_destination_apple.y = tempPos.second;
	g_destination_apple.w = 50;
	g_destination_apple.h = 50;

	//���� ������Ʈ���� �ʱ�ȭ
	InitGameObjectState();

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
	
	SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 0);
	SDL_RenderFillRect(g_renderer, &g_bg_1_source_rect);

	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 0);
	SDL_RenderFillRect(g_renderer, &g_bg_2_source_rect);

	//����� �׸�
	SDL_RenderCopy(g_renderer, g_apple_sheet_texture, &g_apple_source_rect, &g_destination_apple); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�

	// ���� �׸�
	for (auto iter = snakeList.begin(); iter != snakeList.end(); iter++)
	{
		SDL_RenderCopy(g_renderer, g_snake_sheet_texture, &g_snake_source_rect, &iter->destination_snake);
	}

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
			if (event.key.keysym.sym == SDLK_LEFT && g_cur_key != Key::RIGHT) {				
				g_cur_key = Key::LEFT;				
			}
			if (event.key.keysym.sym == SDLK_RIGHT && g_cur_key != Key::LEFT) {
				g_cur_key = Key::RIGHT;
			}
			if (event.key.keysym.sym == SDLK_UP && g_cur_key != Key::DOWN) {
				g_cur_key = Key::UP;
			}
			if (event.key.keysym.sym == SDLK_DOWN && g_cur_key != Key::UP) {
				g_cur_key = Key::DOWN;
			}			
			if (event.key.keysym.sym == SDLK_SPACE) {				
				g_current_game_phase = PHASE_ENDING;				
			}				
			break;
		
		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (!g_stage_flag_running)
				{
					g_current_game_phase = PHASE_ENDING;
					InitGameObjectState();
				}
				/*delete game_phases[g_current_game_phase];			
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
	SDL_DestroyTexture(g_snake_sheet_texture); // �� �޸� ����
	SDL_DestroyTexture(g_apple_sheet_texture); // ��� �޸� ����
	SDL_DestroyTexture(g_gameover_text_kr); // ���ӿ��� �ؽ�Ʈ �޸� ����
	

	TTF_CloseFont(g_font_gameover); // ��Ʈ �޸� ����

	//Mix_FreeChunk(g_missile_fire_sound);
}

void Stage1::InitGameObjectState()
{
	g_cur_key = -1;
	snakeList.clear();
	g_stage_flag_running = true;
	g_stage_is_colliding = false;
	MakeSnake();
}
void Stage1::MakeGameObjTextures()
{
	SDL_Surface* snake_sheet_surface = IMG_Load("../../Resources/snake.png"); // �̹��� ������ ������
	SDL_SetColorKey(snake_sheet_surface, SDL_TRUE, SDL_MapRGB(snake_sheet_surface->format, 255, 255, 255));
	g_snake_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, snake_sheet_surface);

	SDL_Surface* apple_sheet_surface = IMG_Load("../../Resources/apple.png"); // �̹��� ������ ������
	SDL_SetColorKey(apple_sheet_surface, SDL_TRUE, SDL_MapRGB(apple_sheet_surface->format, 255, 255, 255));
	g_apple_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, apple_sheet_surface);
	
	SDL_FreeSurface(snake_sheet_surface);
	SDL_FreeSurface(apple_sheet_surface);
	
}

void Stage1::SnakeMove()
{	
	if (!g_stage_flag_running) return;
	
	Uint32 cur_time_ms = SDL_GetTicks();
	if (cur_time_ms - g_stage_last_time_ms < 170) return; // 200ms���� �� ĭ(50) ������

	if (GetApple())
	{
		MakeSnake();
		g_stage_is_colliding = false;
		cout << "GET!\n";
		pair<int, int> tempPos = CreateRandomPosition();
		g_destination_apple.x = tempPos.first;
		g_destination_apple.y = tempPos.second;
	}
		
	Snake back = snakeList.back();
	
	SDL_Rect curRect = snakeList.front().destination_snake;
	back.destination_snake = curRect;

	if (g_cur_key == Key::LEFT) {
		back.destination_snake.x = curRect.x - CELL;
	}
	else if (g_cur_key == Key::RIGHT) {
		back.destination_snake.x = curRect.x + CELL;
	}
	else if (g_cur_key == Key::UP) {

		back.destination_snake.y = curRect.y - CELL;
	}
	else if (g_cur_key == Key::DOWN) {
		back.destination_snake.y = curRect.y + CELL;
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

void Stage1::MakeSnake()
{
	if (snakeList.empty())
	{
		snakeList.push_front(g_destination_snake);
		return;
	}
	Snake front = snakeList.front();

	if (g_cur_key == Key::LEFT) {
		front.destination_snake.x -= CELL;
	}
	else if (g_cur_key == Key::RIGHT) {
		front.destination_snake.x += CELL;
	}
	else if (g_cur_key == Key::UP) {
		front.destination_snake.y -= CELL;
	}
	else if (g_cur_key == Key::DOWN) {
		front.destination_snake.y += CELL;
	}
	CheckIsSnakeBody();
	if (visited[front.destination_snake.x / 50][front.destination_snake.y / 50])
	{
		g_stage_flag_running = false;
		return;
	}
	snakeList.push_front(front);
}
void Stage1::CheckIsGameOver(SDL_Rect snakeHeadRect)
{
	if ((snakeHeadRect.x < 50 || 600 < snakeHeadRect.x) ||
		(snakeHeadRect.y < 50 || 600 < snakeHeadRect.y))
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
bool Stage1::GetApple()
{
	int snake_x = snakeList.front().destination_snake.x;
	int snake_y = snakeList.front().destination_snake.y;

	int apple_x = g_destination_apple.x;
	int apple_y = g_destination_apple.y;

	if (snake_x == apple_x && snake_y == apple_y)
	{
		if (g_stage_is_colliding) return false;

		g_stage_is_colliding = true;
		return true;
	}
	else return false;
}
pair<int,int> Stage1::CreateRandomPosition()
{
	CheckIsSnakeBody();

	srand((unsigned int)time(NULL));
	
	int x, y;
	while (true)
	{
		int rnd1 = rand(); int rnd2 = rand();
		x = (int)(rnd1 % 12 + 1); y = (int)(rnd2 % 12 + 1);

		if (!visited[x][y]) break;
	}
			
	return { x * CELL, y * CELL };
}
void Stage1::CheckIsSnakeBody()
{	
	fill(&visited[0][0], &visited[12][13], false);

	for (auto iter = snakeList.begin(); iter != snakeList.end(); iter++)
	{
		visited[iter->destination_snake.x / 50][iter->destination_snake.y / 50] = true;
	}
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
