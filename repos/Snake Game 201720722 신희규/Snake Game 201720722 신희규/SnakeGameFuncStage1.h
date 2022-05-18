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
	
	
	SDL_Rect g_bg_source_rect; // ��� �̹������� �߶���� �κ�
	SDL_Texture* g_bg_sheet_texture;
	SDL_Rect g_destination_bg;

	SDL_Rect g_snake_source_rect; // ĳ���� �̹������� �߶���� �κ�
	SDL_Texture* g_snake_sheet_texture;
	SDL_Rect g_destination_snake;

	SDL_Rect g_apple_source_rect; // ��� �̹������� �߶���� �κ�
	SDL_Texture* g_apple_sheet_texture;
	SDL_Rect g_destination_apple;

	TTF_Font* g_font_gameover; // ���� ��Ʈ ����
	SDL_Texture* g_gameover_text_kr;
	SDL_Rect g_gameover_text_kr_rect;
	SDL_Color black = { 0, 0, 0, 0 }; // ���� ����

	int g_cur_key = -1, g_last_key = -1;
	int g_stage_last_time_ms;
	bool g_stage_flag_running, g_stage_is_colliding;
	
	// �귯�� �ð� ���
	double g_elapsed_time_ms;

public:
	Stage1();
	virtual ~Stage1();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

	void DrawGameText();
	void DrawGameOverText();
	void InitChunk();
	void InitTexts();
	void MakeGameObjTextures();
	bool GetApple();
	void SnakeMove();
	void MakeSnake();
	void CheckIsGameOver(SDL_Rect snakeHeadRect);
	void CreateApple();
	pair<int, int> CreateRandomPosition();	
};
