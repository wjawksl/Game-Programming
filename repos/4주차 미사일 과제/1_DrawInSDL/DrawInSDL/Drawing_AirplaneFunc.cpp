#include "Drawing_GameFunc.h"
#include <windows.h>
#include<iostream>
#include"SDL_image.h"

using namespace std;
class Missile {
public :
	bool flag; // ��ü public ����
	SDL_Rect destination_missile;
	SDL_Texture* missile_sheet_texture;

	Missile() = default;
	Missile(bool f, SDL_Rect dm, SDL_Texture* mst) : flag(f), destination_missile(dm), missile_sheet_texture(mst) {}
};

Missile* missile_arr = new Missile[5]; // Ŭ���� �迭 �ʱ�ȭ

int g_key[5];

bool g_missile_flag;
bool g_flag_firing;

Mix_Music* g_bg_mus; // ������� ���� ����
Mix_Chunk* g_missile_fire_sound; // �̻��� �߻� ȿ���� ���� ����

SDL_Texture* g_plane_sheet_texture; // �� �̹������� �ؽ��� ����
SDL_Texture* g_missile_sheet_texture;
SDL_Texture* g_bg_sheet_texture;

SDL_Rect g_bg_source_rect; // ��� �̹������� �߶���� �κ�
SDL_Rect g_destination_bg;

SDL_Rect g_plane_source_rect; // ����� �̹������� �߶���� �κ�
SDL_Rect g_destination_plane;

SDL_Rect g_missile_source_rect; // �̻��� �̹������� �߶���� �κ�
SDL_Rect g_destination_missile;
// �귯�� �ð� ���
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// ���α׷��� ���۵� �� ���ʿ� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���ӿ� �ʿ��� �ڿ�(�̹���, ���� ��)�� �ε��ϰ�, ���� �������� �ʱ�ȭ �ؾ��Ѵ�.
void InitGame() {
	//g_input = 0;
	
	g_flag_running = true;
	g_flag_firing = false;
	
	g_elapsed_time_ms = 0;

	g_bg_mus = Mix_LoadMUS("../../Resources/Kalimba.mp3"); // ������� �ε�
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"Kalimba.mp3\"): " << Mix_GetError() << std::endl;
	}
	g_missile_fire_sound = Mix_LoadWAV("../../Resources/ray_gun-Mike_Koenig-1169060422.wav"); // ȿ���� �ε�

	Mix_VolumeMusic(24);
	Mix_VolumeChunk(g_missile_fire_sound, 24);
	Mix_FadeInMusic(g_bg_mus, -1, 2000); // ������� �÷���

	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.

	SDL_Surface* bg_1_sheet_surface = IMG_Load("../../Resources/back1.jpg"); // �̹��� ������ ������
	g_bg_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_1_sheet_surface);

	SDL_Surface* plane_sheet_surface= IMG_Load("../../Resources/Airplane.png"); // �̹��� ������ ������
	SDL_SetColorKey(plane_sheet_surface, SDL_TRUE, SDL_MapRGB(plane_sheet_surface->format, 255, 255, 255));
	g_plane_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, plane_sheet_surface);

	SDL_Surface* missile_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // �̹��� ������ ������
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);

	SDL_FreeSurface(plane_sheet_surface);

	g_bg_source_rect.x = 0; // ���ȭ��1 ��������
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 500;
	g_bg_source_rect.h = 600;

	g_destination_bg.x = 0; // ���ȭ��1 ��ġ ����
	g_destination_bg.y = 0;
	g_destination_bg.w = 600;
	g_destination_bg.h = 750;
	
	g_plane_source_rect.x = 50; // ����� �߶����
	g_plane_source_rect.y = 305;
	g_plane_source_rect.w = 160;
	g_plane_source_rect.h = 160;

	g_destination_plane.x = 300; // ����� �ʱ� ��ġ
	g_destination_plane.y = 400;
	g_destination_plane.w = 75;
	g_destination_plane.h = 75;

	for (int i = 0; i < 5; i++)
	{
		missile_arr[i] = Missile(false, g_destination_plane, g_missile_sheet_texture);	
	}

	g_missile_source_rect.x = 308; // �̻��� �߶����
	g_missile_source_rect.y = 401;
	g_missile_source_rect.w = 148;
	g_missile_source_rect.h = 91;

	system("cls");
}

/////////////////////////////////////////////////////////////
// Update() 
// ������ ������ ������Ʈ�ϴ� �Լ�.
// ���� ������ ���� �����ؾ��ϴ� ��.
// ���ӿ��� �Ͼ�� ��ȭ�� ��� �� ������ �����Ѵ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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

	if (g_key[0]) {
		g_destination_plane.x -= 10;
	}
	if (g_key[1]) {
		g_destination_plane.x += 10;
	}
	if (g_key[2]) {
		g_destination_plane.y -= 10;
	}
	if (g_key[3]) {
		g_destination_plane.y += 10;
	}
	if (g_key[4]) {
		//for�� ������ flag = false�ΰ͸� ã�Ƴ� �ʱ�ȭ ���ְ� ���� Renderer���� �̹����� �׷��ش�.
		//g_missile_flag = true; // �굵 �迭�̰ڴ� �Ƹ�..
		//������ �ֱ�
		if (g_flag_firing) return;
		g_flag_firing = true;
		Mix_PlayChannel(-1, g_missile_fire_sound, 0);
		for (int i = 0; i < 5; i++)
		{
			if (!missile_arr[i].flag)
			{
				missile_arr[i].flag = true;
				missile_arr[i].destination_missile = g_destination_plane;
				missile_arr[i].destination_missile.y -= 50; // ���⵵ �迭
				
				break;
			}
		}		
	}	
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {
	
	// Background 1
	SDL_RenderCopy(g_renderer, g_bg_sheet_texture, &g_bg_source_rect, &g_destination_bg);

	//g_plane_sheet_texture
	SDL_RenderCopy(g_renderer, g_plane_sheet_texture, &g_plane_source_rect, &g_destination_plane); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�
	
	//���⼭�� for�� ������ flag�� true�� �͸� �׸���!
	for (int i = 0; i < 5; i++)
	{
		if (missile_arr[i].flag)
		{
			//g_missile_sheet_texture
			SDL_RenderCopy(g_renderer, missile_arr[i].missile_sheet_texture, &g_missile_source_rect, &missile_arr[i].destination_missile); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�
		}
	}
	//g_plane_sheet_texture
	//SDL_RenderCopy(g_renderer, g_missile_sheet_texture, &g_missile_source_rect, &g_destination_missile); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�

	SDL_RenderPresent(g_renderer); // �鿡�� �׸� �׸��� �ѹ��� ������
}

/////////////////////////////////////////////////////////////
// HandleEvents() 
// �̺�Ʈ�� ó���ϴ� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
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
				if (g_flag_firing) g_flag_firing = false;
				g_key[4] = 0;
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
void ClearGame()
{
	SDL_DestroyTexture(g_bg_sheet_texture);
	SDL_DestroyTexture(g_plane_sheet_texture); // ����� �޸� ����
	for (int i = 0; i < 5; i++)
	{
		SDL_DestroyTexture(missile_arr[i].missile_sheet_texture); // �̻��� �޸� ����
	}
	Mix_FreeMusic(g_bg_mus);
	Mix_FreeChunk(g_missile_fire_sound);
}

