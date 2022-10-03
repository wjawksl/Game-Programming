#include "Drawing_GameFunc.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // �ѱ��� ������ �ʿ���

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

string g_score_str;

Mix_Music* g_bg_mus; // ������� ���� ����
Mix_Chunk* g_missile_fire_sound; // �̻��� �߻� ȿ���� ���� ����
TTF_Font* g_font; // ���� ��Ʈ ����
SDL_Color white = { 255, 255, 255, 0 }; // ���� ����

SDL_Texture* g_scoreText_kr; // ���� �ؽ�Ʈ(�ѱ�)�� �ؽ��� ����
SDL_Texture* g_scoreText_num; // ���� �ؽ�Ʈ(����)�� �ؽ��� ����

SDL_Texture* g_plane_sheet_texture; // �� �̹������� �ؽ��� ����
SDL_Texture* g_missile_sheet_texture;
SDL_Texture* g_bg_sheet_texture;

SDL_Rect g_bg_source_rect; // ��� �̹������� �߶���� �κ�
SDL_Rect g_destination_bg;

SDL_Rect g_plane_source_rect; // ����� �̹������� �߶���� �κ�
SDL_Rect g_destination_plane;

SDL_Rect g_missile_source_rect; // �̻��� �̹������� �߶���� �κ�
SDL_Rect g_destination_missile;

SDL_Rect g_scoreText_kr_rect; // �ؽ�Ʈ�� �������� �κ�
SDL_Rect g_scoreText_num_rect; // �ؽ�Ʈ�� �������� �κ�
// �귯�� �ð� ���
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// ���α׷��� ���۵� �� ���ʿ� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���ӿ� �ʿ��� �ڿ�(�̹���, ���� ��)�� �ε��ϰ�, ���� �������� �ʱ�ȭ �ؾ��Ѵ�.
void InitGame() {
	//g_input = 0;
	g_score_str = "00000";
	g_flag_running = true;
	g_flag_firing = false;
	
	g_elapsed_time_ms = 0;

	InitBGM();
	Mix_FadeInMusic(g_bg_mus, -1, 2000); // ������� �÷���

	InitMissileChunk(); // �̻��� ȿ���� �ʱ�ȭ

	InitScoreText(); // ���� �ʱ�ȭ

	MakeGameObjTextures();
	
	g_bg_source_rect.x = 0; // ���ȭ�� ��������
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

	g_missile_source_rect.x = 308; // �̻��� �߶����
	g_missile_source_rect.y = 401;
	g_missile_source_rect.w = 148;
	g_missile_source_rect.h = 91;
	
	for (int i = 0; i < 5; i++)
	{
		missile_arr[i] = Missile(false, g_destination_plane, g_missile_sheet_texture);
	}

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
		FireMissile(); // �̻��� �߻�
	}	
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {
	// ���ȭ��
	SDL_RenderCopy(g_renderer, g_bg_sheet_texture, &g_bg_source_rect, &g_destination_bg);

	// �����
	SDL_RenderCopy(g_renderer, g_plane_sheet_texture, &g_plane_source_rect, &g_destination_plane); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�
		
	//���⼭�� for�� ������ flag�� true�� �͸� �׸���!
	DrawMissile();

	// ����
	DrawScoreText();

	// �鿡�� �׸� �׸��� �ѹ��� ������
	SDL_RenderPresent(g_renderer); 
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
	SDL_DestroyTexture(g_scoreText_kr); // �ѱ� ���� �ؽ��� �޸� ����
	SDL_DestroyTexture(g_scoreText_num); // �ѱ� ���� �ؽ��� �޸� ����
	TTF_CloseFont(g_font); // ��Ʈ �޸� ����

	for (int i = 0; i < 5; i++)
	{
		SDL_DestroyTexture(missile_arr[i].missile_sheet_texture); // �̻��� �޸� ����
	}
	Mix_FreeMusic(g_bg_mus);
	Mix_FreeChunk(g_missile_fire_sound);
}

void MakeGameObjTextures()
{
	SDL_Surface* bg_1_sheet_surface = IMG_Load("../../Resources/back1.jpg"); // �̹��� ������ ������
	g_bg_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_1_sheet_surface);

	SDL_Surface* plane_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // �̹��� ������ ������
	SDL_SetColorKey(plane_sheet_surface, SDL_TRUE, SDL_MapRGB(plane_sheet_surface->format, 255, 255, 255));
	g_plane_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, plane_sheet_surface);

	SDL_Surface* missile_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // �̹��� ������ ������
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);

	SDL_FreeSurface(bg_1_sheet_surface);
	SDL_FreeSurface(plane_sheet_surface);
	SDL_FreeSurface(missile_sheet_surface);
}

void DrawMissile()
{
	for (int i = 0; i < 5; i++)
	{
		if (missile_arr[i].flag)
		{
			//g_missile_sheet_texture
			SDL_RenderCopy(g_renderer, missile_arr[i].missile_sheet_texture, &g_missile_source_rect, &missile_arr[i].destination_missile); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�
		}
	}
}

void FireMissile()
{
	if (g_flag_firing) return;
	g_flag_firing = true;

	for (int i = 0; i < 5; i++)
	{
		if (!missile_arr[i].flag)
		{
			missile_arr[i].flag = true;
			missile_arr[i].destination_missile = g_destination_plane;
			missile_arr[i].destination_missile.y -= 50; // �̻��� ��ġ ����

			int score_int = std::stoi(g_score_str);
			score_int += 10;
			g_score_str = std::to_string(score_int);
			while (g_score_str.size() != 5)
				g_score_str = "0" + g_score_str; // ���ھ� ����

			Mix_PlayChannel(-1, g_missile_fire_sound, 0); //�߻� ȿ����
			break;
		}
	}
}

void DrawScoreText()
{
	// Draw Score (Korean)
	// ȭ�鿡 ǥ�� �� ��ġ
	SDL_Rect tmp_r;
	tmp_r.x = 25;
	tmp_r.y = 0;
	tmp_r.w = g_scoreText_kr_rect.w;
	tmp_r.h = g_scoreText_kr_rect.h;
	SDL_RenderCopy(g_renderer, g_scoreText_kr, &g_scoreText_kr_rect, &tmp_r);

	// Draw Score (Number)
	const char* cstr = g_score_str.c_str();
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(g_font, cstr, white);

	g_scoreText_num_rect.x = 0;
	g_scoreText_num_rect.y = 0;
	g_scoreText_num_rect.w = tmp_surface->w;
	g_scoreText_num_rect.h = tmp_surface->h;

	g_scoreText_num = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface); // ��� �����������

	tmp_r.x = 85;
	tmp_r.y = 0;
	tmp_r.w = g_scoreText_num_rect.w;
	tmp_r.h = g_scoreText_num_rect.h;

	SDL_RenderCopy(g_renderer, g_scoreText_num, &g_scoreText_num_rect, &tmp_r);
	SDL_DestroyTexture(g_scoreText_num); // ��� �����������
}
void InitBGM()
{
	g_bg_mus = Mix_LoadMUS("../../Resources/Retrocity.mp3"); // ������� �ε�
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"Kalimba.mp3\"): " << Mix_GetError() << std::endl;
	}
	Mix_VolumeMusic(24);
}
void InitMissileChunk()
{
	g_missile_fire_sound = Mix_LoadWAV("../../Resources/fire.mp3"); // ȿ���� �ε�
	Mix_VolumeChunk(g_missile_fire_sound, 24);
}
void InitScoreText()
{
	// scoreText_kr Title
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 24);
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(g_font, CW2A(L"����: ", CP_UTF8), white);
	//�ؽ�Ʈ ��������
	g_scoreText_kr_rect.x = 0;
	g_scoreText_kr_rect.y = 0;
	g_scoreText_kr_rect.w = tmp_surface->w;
	g_scoreText_kr_rect.h = tmp_surface->h;

	g_scoreText_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);

	// scoreText_num
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28); // �������� ����
}
