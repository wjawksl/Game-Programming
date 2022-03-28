#include "Drawing_GameFunc.h"
#include <windows.h>
#include<iostream>
#include"SDL_image.h"

using namespace std;
class Missile { // Ŭ���� �����ϰ�(�̻��� ��ü) ����ü �迭�� ���� �� SDL_Texture�� �����ϸ� ���ɵɱ�? �迭������ flag�� false�� �ٲ��ְ�
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

Missile* missile_arr = new Missile[5]; // Ŭ���� �迭 �ʱ�ȭ

int g_input;
bool g_missile_flag;
bool g_flag_firing;

SDL_Texture* g_plane_sheet_texture; // �� �̹������� �ؽ��� ����
SDL_Texture* g_missile_sheet_texture;
SDL_Texture* g_bg_1_sheet_texture;

SDL_Rect g_bg_1_source_rect; // ��� �̹������� �߶���� �κ�
SDL_Rect g_destination_bg_1;

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
	g_input = 0;
	
	g_flag_running = true;
	g_flag_firing = false;
	
	g_elapsed_time_ms = 0;

	// Clear the console screen.
	// ǥ����� ȭ���� ������ �����.

	SDL_Surface* bg_1_sheet_surface = IMG_Load("../../Resources/back1.jpg"); // �̹��� ������ ������
	g_bg_1_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_1_sheet_surface);

	SDL_Surface* plane_sheet_surface= IMG_Load("../../Resources/Airplane.png"); // �̹��� ������ ������
	SDL_SetColorKey(plane_sheet_surface, SDL_TRUE, SDL_MapRGB(plane_sheet_surface->format, 255, 255, 255));
	g_plane_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, plane_sheet_surface);

	SDL_Surface* missile_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // �̹��� ������ ������
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);

	SDL_FreeSurface(plane_sheet_surface);

	g_bg_1_source_rect.x = 0; // ���ȭ��1 ��������
	g_bg_1_source_rect.y = 0;
	g_bg_1_source_rect.w = 500;
	g_bg_1_source_rect.h = 600;

	g_destination_bg_1.x = 0; // ���ȭ��1 ��ġ ����
	g_destination_bg_1.y = 0;
	g_destination_bg_1.w = 600;
	g_destination_bg_1.h = 750;
	
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
		//for�� ������ flag = false�ΰ͸� ã�Ƴ� �ʱ�ȭ ���ְ� ���� Renderer���� �̹����� �׷��ش�.
		//g_missile_flag = true; // �굵 �迭�̰ڴ� �Ƹ�..
		//������ �ֱ�
		if (g_flag_firing) return;
		g_flag_firing = true;

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
		/*g_destination_missile = g_destination_plane;
		g_destination_missile.y = g_destination_plane.y - 50; // ���⵵ �迭*/
	}

	
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {
	
	// Background 1
	SDL_RenderCopy(g_renderer, g_bg_1_sheet_texture, &g_bg_1_source_rect, &g_destination_bg_1);

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
// ���α׷��� ���� �� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���� �ڿ�(�̹���, ���� ��)�� �޸� ���� �����ؾ��Ѵ�.
void ClearGame()
{
	SDL_DestroyTexture(g_plane_sheet_texture); // �޸� ����
	for (int i = 0; i < 5; i++)
	{
		SDL_DestroyTexture(missile_arr[i].missile_sheet_texture);
	}
}

