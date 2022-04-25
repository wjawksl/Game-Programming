#include "Drawing_GameFunc.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // �ѱ��� ������ �ʿ���

#define SIZE 8

using namespace std;

class Missile {
public:
	bool flag; // ��ü public ����
	int angle;
	SDL_Rect destination_missile;
	SDL_Texture* missile_sheet_texture;

	Missile() = default;
	Missile(bool f, int a, SDL_Rect dm, SDL_Texture* mst) : flag(f), angle(a), destination_missile(dm), missile_sheet_texture(mst) {}
};

Missile* missile_arr = new Missile[SIZE]; // Ŭ���� �迭 �ʱ�ȭ

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


/////////////////////////////////////////////////////////////
// InitGame() 
// ���α׷��� ���۵� �� ���ʿ� �� �� ȣ��Ǵ� �Լ�.
// �� �Լ����� ���ӿ� �ʿ��� �ڿ�(�̹���, ���� ��)�� �ε��ϰ�, ���� �������� �ʱ�ȭ �ؾ��Ѵ�.
void InitGame() {
	//g_input = 0;
	g_missile_cnt = 0;
	g_flag_running = true;
	g_flag_firing = false;
	g_flag_interaction = false;
	g_flag_boarding = false;
	g_elapsed_time_ms = 0;
	g_tank_angle = 0;

	InitBGM();
	//Mix_FadeInMusic(g_bg_mus, -1, 2000); // ������� �÷���

	InitChunk(); //ȿ���� �ʱ�ȭ

	InitTexts(); // ���� �ʱ�ȭ

	MakeGameObjTextures(); // ������Ʈ���� �ؽ�Ʈ ��������

	g_bg_source_rect.x = 0; // ���ȭ�� ��������
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 447;
	g_bg_source_rect.h = 446;

	g_destination_bg.x = 0; // ���ȭ��1 ��ġ ����
	g_destination_bg.y = 0;
	g_destination_bg.w = 700;
	g_destination_bg.h = 700;

	g_tank_source_rect.x = 206; // ��ũ �߶����
	g_tank_source_rect.y = 0;
	g_tank_source_rect.w = 88;
	g_tank_source_rect.h = 102;

	g_destination_tank.x = 315; // ��ũ �ʱ� ��ġ
	g_destination_tank.y = 315;
	g_destination_tank.w = 75;
	g_destination_tank.h = 75;

	g_missile_source_rect.x = 349; // �̻��� �߶����
	g_missile_source_rect.y = 1453;
	g_missile_source_rect.w = 69;
	g_missile_source_rect.h = 88;

	g_charactor_source_rect = { 182, 5, 356, 582 }; // ĳ���� �߶����
	g_destination_charactor = { 200, 200, 42, 66 }; // ĳ������ ��ġ

	g_box_source_rect = { 233, 257, 1139, 885 }; // �������� �߶����
	g_destination_box = { 50, 100, 80, 64 }; // ���������� ��ġ

	for (int i = 0; i < SIZE; i++)
	{
		missile_arr[i] = Missile(false, -1, g_destination_tank, g_missile_sheet_texture);
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
	
	g_elapsed_time_ms += 33;

	UpdateMissile();

	if (!g_flag_boarding)
	{
		PlayerMove();
		if (g_key[4]) {

			if (g_flag_interaction) return;
			g_flag_interaction = true;
			
			if (DistinctObject(g_destination_tank)) // ��ũ�� ��ȣ�ۿ��̶��
			{				
				Mix_PlayChannel(-1, g_ride_tank_sound, 0);
				g_flag_boarding = true;
			}
			else if (DistinctObject(g_destination_box)) // ���� ���ڿ� ��ȣ�ۿ��̶��
			{				
				Mix_PlayChannel(-1, g_open_box_sound, 0);
				g_missile_cnt += 5;
			}
		}
	}
	else
	{	
		TankMoveAndFire();
		if (g_key[4])
		{
			if (g_flag_interaction) return;
			g_flag_interaction = true;

			Mix_PlayChannel(-1, g_ride_tank_sound, 0);
			g_flag_boarding = false;
		}
	}
}

/////////////////////////////////////////////////////////////
// Render() 
// �׸��� �׸��� �Լ�.
// main �Լ��� while loop�� ���ؼ� ������ �ݺ� ȣ��ȴٴ� ���� ����.
void Render() {
	// ���ȭ��
	SDL_RenderCopy(g_renderer, g_bg_sheet_texture, &g_bg_source_rect, &g_destination_bg); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�

	// ��ũ
	SDL_RenderCopyEx(g_renderer, g_tank_sheet_texture, &g_tank_source_rect,
		&g_destination_tank, 90 * g_tank_angle, NULL, SDL_FLIP_NONE);

	// ��������
	if(!g_missile_cnt)
		SDL_RenderCopy(g_renderer, g_box_sheet_texture, &g_box_source_rect, &g_destination_box);

	// ĳ����
	if(!g_flag_boarding)
		SDL_RenderCopy(g_renderer, g_charactor_sheet_texture, &g_charactor_source_rect, &g_destination_charactor);

	//���⼭�� for�� ������ flag�� true�� �͸� �׸���!
	DrawMissile();

	// �ؽ�Ʈ
	DrawGameText();

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
			if (g_flag_boarding && g_flag_firing) g_flag_firing = false;

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
				if (g_flag_interaction) g_flag_interaction = false;
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
	SDL_DestroyTexture(g_tank_sheet_texture); // ��ũ �޸� ����
	SDL_DestroyTexture(g_box_sheet_texture); // ���� ���� �޸� ����
	SDL_DestroyTexture(g_charactor_sheet_texture); // ĳ���� �޸� ����
	SDL_DestroyTexture(g_missile_cnt_texture); // �ܿ� �̻��� �� �ؽ��� �޸� ����
	TTF_CloseFont(g_font); // ��Ʈ �޸� ����

	for (int i = 0; i < SIZE; i++)
	{
		SDL_DestroyTexture(missile_arr[i].missile_sheet_texture); // �̻��� �޸� ����
	}
	Mix_FreeMusic(g_bg_mus);
	Mix_FreeChunk(g_missile_fire_sound);
	Mix_FreeChunk(g_open_box_sound);
	Mix_FreeChunk(g_ride_tank_sound);
}

void MakeGameObjTextures()
{
	SDL_Surface* bg_sheet_surface = IMG_Load("../../Resources/BG.png"); // �̹��� ������ ������
	g_bg_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_sheet_surface);

	SDL_Surface* tank_sheet_surface = IMG_Load("../../Resources/Tank.png"); // �̹��� ������ ������
	SDL_SetColorKey(tank_sheet_surface, SDL_TRUE, SDL_MapRGB(tank_sheet_surface->format, 255, 255, 255));
	g_tank_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, tank_sheet_surface);

	SDL_Surface* missile_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // �̹��� ������ ������
	SDL_SetColorKey(missile_sheet_surface, SDL_TRUE, SDL_MapRGB(missile_sheet_surface->format, 255, 255, 255));
	g_missile_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, missile_sheet_surface);

	SDL_Surface* charactor_sheet_surface = IMG_Load("../../Resources/charactor.png"); // �̹��� ������ ������
	SDL_SetColorKey(charactor_sheet_surface, SDL_TRUE, SDL_MapRGB(charactor_sheet_surface->format, 255, 255, 255));
	g_charactor_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, charactor_sheet_surface);

	SDL_Surface* box_sheet_surface = IMG_Load("../../Resources/box.jpg"); // �̹��� ������ ������
	SDL_SetColorKey(box_sheet_surface, SDL_TRUE, SDL_MapRGB(box_sheet_surface->format, 255, 255, 255));
	g_box_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, box_sheet_surface);
	
	SDL_FreeSurface(charactor_sheet_surface);
	SDL_FreeSurface(box_sheet_surface);
	SDL_FreeSurface(bg_sheet_surface);
	SDL_FreeSurface(tank_sheet_surface);
	SDL_FreeSurface(missile_sheet_surface);
}

void DrawMissile()
{
	for (int i = 0; i < SIZE; i++)
	{
		if (missile_arr[i].flag)
		{
			//g_missile_sheet_texture
			SDL_RenderCopyEx(g_renderer, missile_arr[i].missile_sheet_texture,
				&g_missile_source_rect, &missile_arr[i].destination_missile, 90 * (double)missile_arr[i].angle, NULL, SDL_FLIP_NONE); // texture�� �����ؼ� ȭ�鿡 ��Ÿ���ִ� �Լ�
		}
	}
}
void PlayerMove()
{
	if (g_key[0]) {
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
	}	
}
void TankMoveAndFire()
{
	if (g_key[0]) {
		g_tank_angle = 3;

		if(g_missile_cnt > 0)
			FireMissile();
	}
	if (g_key[1]) {
		g_tank_angle = 1;
		
		if (g_missile_cnt > 0)
			FireMissile();
	}
	if (g_key[2]) {
		g_tank_angle = 0;
		
		if (g_missile_cnt > 0)
			FireMissile();
	}
	if (g_key[3]) {
		g_tank_angle = 2;
		
		if (g_missile_cnt > 0)
			FireMissile();
	}
	
}
bool DistinctObject(SDL_Rect rect)
{
	int player_x = g_destination_charactor.x;
	int player_y = g_destination_charactor.y;

	int obj_rect_x_src = rect.x - rect.w / 2;
	int obj_rect_y_src = rect.y - rect.h / 2;

	int obj_rect_x_dst = rect.x + rect.w / 2;
	int obj_rect_y_dst = rect.y + rect.h / 2;

	if (player_x > obj_rect_x_src && player_y > obj_rect_y_src &&
		player_x < obj_rect_x_dst && player_y < obj_rect_y_dst) return true;

	return false;
}
void FireMissile()
{
	if (g_flag_firing) return;
	g_flag_firing = true;

	for (int i = 0; i < SIZE; i++)
	{
		if (!missile_arr[i].flag)
		{
			missile_arr[i].flag = true;
			missile_arr[i].destination_missile.x = g_destination_tank.x + 75 * dx[(int)g_tank_angle];
			missile_arr[i].destination_missile.y = g_destination_tank.y + 75 * dy[(int)g_tank_angle]; // �̻��� ��ġ ����
			missile_arr[i].destination_missile.w = 56;
			missile_arr[i].destination_missile.h = 72;
			missile_arr[i].angle = (int)g_tank_angle;

			Mix_PlayChannel(-1, g_missile_fire_sound, 0); //�߻� ȿ����
			g_missile_cnt--;
			break;
		}
	}
}

void UpdateMissile()
{
	for (int i = 0; i < SIZE; i++) // �̻����� ������ ����
	{
		if (missile_arr[i].flag)
		{
			missile_arr[i].destination_missile.x += 15 * dx[missile_arr[i].angle];
			missile_arr[i].destination_missile.y += 15 * dy[missile_arr[i].angle];

			if (missile_arr[i].destination_missile.x < 0 || 650 < missile_arr[i].destination_missile.x ||
				missile_arr[i].destination_missile.y < 0 || 650 < missile_arr[i].destination_missile.y)
			{
				missile_arr[i].flag = false;
				missile_arr[i].angle = -1;
			}
		}
	}
}

void DrawGameText()
{	
	// Draw Score (Korean)
	SDL_Rect tmp_r; // ȭ�鿡 ǥ�� �� ��ġ

	if(g_flag_boarding)
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
	SDL_DestroyTexture(g_missile_cnt_texture); // ��� �����������
}
void InitBGM()
{
	g_bg_mus = Mix_LoadMUS("../../Resources/Retrocity.mp3"); // ������� �ε�
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"Kalimba.mp3\"): " << Mix_GetError() << std::endl;
	}
	Mix_VolumeMusic(24);
}
void InitChunk()
{
	g_missile_fire_sound = Mix_LoadWAV("../../Resources/fire.mp3"); // ȿ���� �ε�
	Mix_VolumeChunk(g_missile_fire_sound, 24);

	g_open_box_sound = Mix_LoadWAV("../../Resources/Coin.wav"); // ȿ���� �ε�
	Mix_VolumeChunk(g_open_box_sound, 24);

	g_ride_tank_sound = Mix_LoadWAV("../../Resources/IO.mp3"); // ȿ���� �ε�
	Mix_VolumeChunk(g_ride_tank_sound, 24);
	
}
void InitTexts()
{	
	// scoreText_kr Title
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
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28); // �������� ����
}
