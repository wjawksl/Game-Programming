#pragma once

#include "GameFunc.h"

class Stage1 : public PhaseInterface
{
public:
	Stage1();
	virtual ~Stage1();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

	void FireMissile();
	void DrawGameText();
	void DrawMissile();
	void InitBGM();
	void InitChunk();
	void InitTexts();
	void MakeGameObjTextures();
	bool DistinctObject(SDL_Rect rect);
	void PlayerMove();
	void TankMoveAndFire();
	void UpdateMissile();

private:
	class Missile {
	public:
		bool flag; // 전체 public 선언
		int angle;
		SDL_Rect destination_missile;
		SDL_Texture* missile_sheet_texture;

		Missile() = default;
		Missile(bool f, int a, SDL_Rect dm, SDL_Texture* mst) : flag(f), angle(a), destination_missile(dm), missile_sheet_texture(mst) {}
		~Missile() = default;
		
		//flag 확인, angle변경, flag 변경 함수
	};

	Missile* missile_arr;

	int g_key[5];
	int dx[4] = { 0,1,0,-1 }; // 위, 오른쪽, 아래, 왼쪽
	int dy[4] = { -1,0,1,0 };

	bool g_missile_flag;
	bool g_flag_firing;
	bool g_flag_interaction;
	bool g_flag_boarding;

	int g_missile_cnt;
	double g_tank_angle;

	Mix_Music* g_bg_mus; // 배경음악 변수 선언

	Mix_Chunk* g_missile_fire_sound; // 미사일 발사 효과음 변수 선언
	Mix_Chunk* g_open_box_sound; // 상자 개봉 효과음
	Mix_Chunk* g_ride_tank_sound;

	TTF_Font* g_font; // 게임 폰트 선언
	SDL_Color black = { 0, 0, 0, 0 }; // 색깔 선언

	SDL_Texture* g_missile_cnt_texture; // 잔여 미사일 수의 텍스쳐 선언

	SDL_Texture* g_tank_sheet_texture; // 각 이미지들의 텍스쳐 선언
	SDL_Texture* g_missile_sheet_texture;
	SDL_Texture* g_bg_sheet_texture;

	SDL_Rect g_bg_source_rect; // 배경 이미지에서 잘라오는 부분
	SDL_Rect g_destination_bg;

	SDL_Rect g_tank_source_rect; // 비행기 이미지에서 잘라오는 부분
	SDL_Rect g_destination_tank;

	SDL_Rect g_missile_source_rect; // 미사일 이미지에서 잘라오는 부분
	SDL_Rect g_destination_missile;

	SDL_Rect g_charactor_source_rect; // 캐릭터 이미지에서 잘라오는 부분
	SDL_Rect g_destination_charactor;
	SDL_Texture* g_charactor_sheet_texture;

	SDL_Rect g_box_source_rect; // 캐릭터 이미지에서 잘라오는 부분
	SDL_Rect g_destination_box;
	SDL_Texture* g_box_sheet_texture;

	SDL_Rect g_missile_cnt_rect; // 텍스트를 가져오는 부분

	SDL_Texture* g_board_text_kr; // 탑승중 텍스트를 가져옴
	SDL_Rect g_board_text_kr_rect;
	// 흘러간 시간 기록
	double g_elapsed_time_ms;

};



