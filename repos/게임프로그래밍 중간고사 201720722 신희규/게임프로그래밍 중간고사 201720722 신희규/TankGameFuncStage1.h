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
		bool flag; // ��ü public ����
		int angle;
		SDL_Rect destination_missile;
		SDL_Texture* missile_sheet_texture;

		Missile() = default;
		Missile(bool f, int a, SDL_Rect dm, SDL_Texture* mst) : flag(f), angle(a), destination_missile(dm), missile_sheet_texture(mst) {}
		~Missile() = default;
		
		//flag Ȯ��, angle����, flag ���� �Լ�
	};

	Missile* missile_arr;

	int g_key[5];
	int dx[4] = { 0,1,0,-1 }; // ��, ������, �Ʒ�, ����
	int dy[4] = { -1,0,1,0 };

	bool g_missile_flag;
	bool g_flag_firing;
	bool g_flag_interaction;
	bool g_flag_boarding;

	int g_missile_cnt;
	double g_tank_angle;

	Mix_Music* g_bg_mus; // ������� ���� ����

	Mix_Chunk* g_missile_fire_sound; // �̻��� �߻� ȿ���� ���� ����
	Mix_Chunk* g_open_box_sound; // ���� ���� ȿ����
	Mix_Chunk* g_ride_tank_sound;

	TTF_Font* g_font; // ���� ��Ʈ ����
	SDL_Color black = { 0, 0, 0, 0 }; // ���� ����

	SDL_Texture* g_missile_cnt_texture; // �ܿ� �̻��� ���� �ؽ��� ����

	SDL_Texture* g_tank_sheet_texture; // �� �̹������� �ؽ��� ����
	SDL_Texture* g_missile_sheet_texture;
	SDL_Texture* g_bg_sheet_texture;

	SDL_Rect g_bg_source_rect; // ��� �̹������� �߶���� �κ�
	SDL_Rect g_destination_bg;

	SDL_Rect g_tank_source_rect; // ����� �̹������� �߶���� �κ�
	SDL_Rect g_destination_tank;

	SDL_Rect g_missile_source_rect; // �̻��� �̹������� �߶���� �κ�
	SDL_Rect g_destination_missile;

	SDL_Rect g_charactor_source_rect; // ĳ���� �̹������� �߶���� �κ�
	SDL_Rect g_destination_charactor;
	SDL_Texture* g_charactor_sheet_texture;

	SDL_Rect g_box_source_rect; // ĳ���� �̹������� �߶���� �κ�
	SDL_Rect g_destination_box;
	SDL_Texture* g_box_sheet_texture;

	SDL_Rect g_missile_cnt_rect; // �ؽ�Ʈ�� �������� �κ�

	SDL_Texture* g_board_text_kr; // ž���� �ؽ�Ʈ�� ������
	SDL_Rect g_board_text_kr_rect;
	// �귯�� �ð� ���
	double g_elapsed_time_ms;

};



