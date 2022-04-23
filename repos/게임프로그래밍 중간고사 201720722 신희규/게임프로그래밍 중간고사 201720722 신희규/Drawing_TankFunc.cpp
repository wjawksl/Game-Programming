#include "Drawing_GameFunc.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // �ѱ��� ������ �ʿ���

using namespace std;

class Missile {
public:
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
bool g_flag_interaction;

int g_missile_cnt;

Mix_Music* g_bg_mus; // ������� ���� ����
Mix_Chunk* g_missile_fire_sound; // �̻��� �߻� ȿ���� ���� ����
TTF_Font* g_font; // ���� ��Ʈ ����
SDL_Color black = { 0, 0, 0, 0 }; // ���� ����

SDL_Texture* g_scoreText_num; // ���� �ؽ�Ʈ(����)�� �ؽ��� ����

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

SDL_Rect g_scoreText_num_rect; // �ؽ�Ʈ�� �������� �κ�
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
	g_elapsed_time_ms = 0;

	InitBGM();
	//Mix_FadeInMusic(g_bg_mus, -1, 2000); // ������� �÷���

	InitMissileChunk(); // �̻��� ȿ���� �ʱ�ȭ

	InitScoreText(); // ���� �ʱ�ȭ

	MakeGameObjTextures();

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

	for (int i = 0; i < 5; i++)
	{
		missile_arr[i] = Missile(false, g_destination_tank, g_missile_sheet_texture);
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
	if (g_key[4]) {

		if (g_flag_interaction) return;
		g_flag_interaction = true;

		//FireMissile(); // �̻��� �߻�

		if (ObjectDistinct(g_destination_tank))
		{
			std::cout << "Tank!" << std::endl;					
		}
		else if (ObjectDistinct(g_destination_box))
		{
			std::cout << "Box!" << std::endl;
			g_missile_cnt += 5;
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
	 	
	// �����
	SDL_RenderCopy(g_renderer, g_tank_sheet_texture, &g_tank_source_rect, &g_destination_tank); 

	// ��������
	SDL_RenderCopy(g_renderer, g_box_sheet_texture, &g_box_source_rect, &g_destination_box);

	// ĳ����
	SDL_RenderCopy(g_renderer, g_charactor_sheet_texture, &g_charactor_source_rect, &g_destination_charactor);

	//���⼭�� for�� ������ flag�� true�� �͸� �׸���!
	DrawMissile();

	// ����
	DrawMissileCntText();

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
	SDL_FreeSurface(bg_sheet_surface);
	SDL_FreeSurface(tank_sheet_surface);
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

bool ObjectDistinct(SDL_Rect rect)
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

	for (int i = 0; i < 5; i++)
	{
		if (!missile_arr[i].flag)
		{
			missile_arr[i].flag = true;
			missile_arr[i].destination_missile.x = g_destination_tank.x + 10;
			missile_arr[i].destination_missile.y = g_destination_tank.y - 75; // �̻��� ��ġ ����
			missile_arr[i].destination_missile.w = 56;
			missile_arr[i].destination_missile.h = 72;
			
			Mix_PlayChannel(-1, g_missile_fire_sound, 0); //�߻� ȿ����
			break;
		}
	}
}

void DrawMissileCntText()
{
	// ȭ�鿡 ǥ�� �� ��ġ
	SDL_Rect tmp_r;

	// Draw Score (Number)
	string missile_cnt_str = to_string(g_missile_cnt);
	const char* cstr = missile_cnt_str.c_str();
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(g_font, cstr, black);

	g_scoreText_num_rect.x = 0;
	g_scoreText_num_rect.y = 0;
	g_scoreText_num_rect.w = tmp_surface->w;
	g_scoreText_num_rect.h = tmp_surface->h;

	g_scoreText_num = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface); // ��� �����������

	tmp_r.x = 650;
	tmp_r.y = 650;
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
	// scoreText_num
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28); // �������� ����
}
