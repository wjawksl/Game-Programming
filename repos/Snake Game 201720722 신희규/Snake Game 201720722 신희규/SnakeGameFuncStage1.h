#pragma once

#include "GameFunc.h"
#include<list>

using namespace std;

class Stage1 : public PhaseInterface
{
private:
	class Snake {
	private:

	public:				
		SDL_Rect destination_snake;

		Snake(SDL_Rect ds)
			: destination_snake(ds)
		{}
	};
	list<Snake> snakeList;
	
	
	SDL_Rect g_bg_source_rect; // 배경 이미지에서 잘라오는 부분
	SDL_Texture* g_bg_sheet_texture;
	SDL_Rect g_destination_bg;

	SDL_Rect g_snake_source_rect; // 캐릭터 이미지에서 잘라오는 부분
	SDL_Texture* g_snake_sheet_texture;
	SDL_Rect g_destination_snake;

	SDL_Rect g_apple_source_rect; // 사과 이미지에서 잘라오는 부분
	SDL_Texture* g_apple_sheet_texture;
	SDL_Rect g_destination_apple;

	TTF_Font* g_font_gameover; // 게임 폰트 선언
	SDL_Texture* g_gameover_text_kr;
	SDL_Rect g_gameover_text_kr_rect;
	SDL_Color black = { 0, 0, 0, 0 }; // 색깔 선언

	int g_cur_key = -1, g_last_key = -1;
	int g_stage_last_time_ms;
	bool g_stage_flag_running;
	/*int dx[4] = {0,1,0,-1}; // 위, 오른쪽, 아래, 왼쪽
	int dy[4] = { -1,0,1,0 };

	bool g_missile_flag;
	bool g_flag_firing;
	bool g_flag_interaction;
	bool g_flag_boarding;

	int g_missile_cnt;
	double g_tank_angle;*/

	/*Mix_Music* g_bg_mus; // 배경음악 변수 선언

	Mix_Chunk* g_missile_fire_sound; // 미사일 발사 효과음 변수 선언
	Mix_Chunk* g_open_box_sound; // 상자 개봉 효과음
	Mix_Chunk* g_ride_tank_sound;

	TTF_Font* g_font; // 게임 폰트 선언
	SDL_Color black = { 0, 0, 0, 0 }; // 색깔 선언

	SDL_Texture* g_missile_cnt_texture; // 잔여 미사일 수의 텍스쳐 선언

	SDL_Texture* g_tank_sheet_texture; // 각 이미지들의 텍스쳐 선언
	SDL_Texture* g_missile_sheet_texture;


	SDL_Rect g_tank_source_rect; // 비행기 이미지에서 잘라오는 부분
	SDL_Rect g_destination_tank;

	SDL_Rect g_missile_source_rect; // 미사일 이미지에서 잘라오는 부분
	SDL_Rect g_destination_missile;
	*/
	

	/*
	SDL_Rect g_box_source_rect; // 캐릭터 이미지에서 잘라오는 부분
	SDL_Rect g_destination_box;
	SDL_Texture* g_box_sheet_texture;

	SDL_Rect g_missile_cnt_rect; // 텍스트를 가져오는 부분

	SDL_Texture* g_board_text_kr; // 탑승중 텍스트를 가져옴
	SDL_Rect g_board_text_kr_rect;*/
	// 흘러간 시간 기록
	double g_elapsed_time_ms;

public:
	Stage1();
	virtual ~Stage1();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

	//void FireMissile();
	void DrawGameText();
	void DrawGameOverText();
	//void DrawMissile();
	void InitChunk();
	void InitTexts();
	void MakeGameObjTextures();
	bool DistinctObject(SDL_Rect rect);
	void SnakeMove();
	void MakeSnake(Snake snake);
	void CheckIsGameOver(SDL_Rect snakeHeadRect);
	void CreateApple();
	//void TankMoveAndFire();
	//void UpdateMissile();



};
