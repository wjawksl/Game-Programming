#include "Drawing_GameFunc.h"
#include "SDL_image.h"
#include <windows.h>
#include <string>
#include <atlstr.h> // 한국어 쓰려면 필요함

using namespace std;

class Missile {
public :
	bool flag; // 전체 public 선언
	SDL_Rect destination_missile;
	SDL_Texture* missile_sheet_texture;

	Missile() = default;
	Missile(bool f, SDL_Rect dm, SDL_Texture* mst) : flag(f), destination_missile(dm), missile_sheet_texture(mst) {}
};

Missile* missile_arr = new Missile[5]; // 클래스 배열 초기화

int g_key[5];

bool g_missile_flag;
bool g_flag_firing;

string g_score_str;

Mix_Music* g_bg_mus; // 배경음악 변수 선언
Mix_Chunk* g_missile_fire_sound; // 미사일 발사 효과음 변수 선언
TTF_Font* g_font; // 게임 폰트 선언
SDL_Color white = { 255, 255, 255, 0 }; // 색깔 선언

SDL_Texture* g_scoreText_kr; // 점수 텍스트(한글)의 텍스쳐 선언
SDL_Texture* g_scoreText_num; // 점수 텍스트(숫자)의 텍스쳐 선언

SDL_Texture* g_plane_sheet_texture; // 각 이미지들의 텍스쳐 선언
SDL_Texture* g_missile_sheet_texture;
SDL_Texture* g_bg_sheet_texture;

SDL_Rect g_bg_source_rect; // 배경 이미지에서 잘라오는 부분
SDL_Rect g_destination_bg;

SDL_Rect g_plane_source_rect; // 비행기 이미지에서 잘라오는 부분
SDL_Rect g_destination_plane;

SDL_Rect g_missile_source_rect; // 미사일 이미지에서 잘라오는 부분
SDL_Rect g_destination_missile;

SDL_Rect g_scoreText_kr_rect; // 텍스트를 가져오는 부분
SDL_Rect g_scoreText_num_rect; // 텍스트를 가져오는 부분
// 흘러간 시간 기록
double g_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void InitGame() {
	//g_input = 0;
	g_score_str = "00000";
	g_flag_running = true;
	g_flag_firing = false;
	
	g_elapsed_time_ms = 0;

	InitBGM();
	Mix_FadeInMusic(g_bg_mus, -1, 2000); // 배경음악 플레이

	InitMissileChunk(); // 미사일 효과음 초기화

	InitScoreText(); // 점수 초기화

	MakeGameObjTextures();
	
	g_bg_source_rect.x = 0; // 배경화면 가져오기
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 500;
	g_bg_source_rect.h = 600;

	g_destination_bg.x = 0; // 배경화면1 위치 설정
	g_destination_bg.y = 0;
	g_destination_bg.w = 600;
	g_destination_bg.h = 750;
	
	g_plane_source_rect.x = 50; // 비행기 잘라오기
	g_plane_source_rect.y = 305;
	g_plane_source_rect.w = 160;
	g_plane_source_rect.h = 160;

	g_destination_plane.x = 300; // 비행기 초기 위치
	g_destination_plane.y = 400;
	g_destination_plane.w = 75;
	g_destination_plane.h = 75;

	g_missile_source_rect.x = 308; // 미사일 잘라오기
	g_missile_source_rect.y = 401;
	g_missile_source_rect.w = 148;
	g_missile_source_rect.h = 91;
	
	for (int i = 0; i < 5; i++)
	{
		missile_arr[i] = Missile(false, g_destination_plane, g_missile_sheet_texture);
	}

	// Clear the console screen.
	// 표준출력 화면을 깨끗히 지운다.
	system("cls");
}

/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
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
		FireMissile(); // 미사일 발사
	}	
}

/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {
	// 배경화면
	SDL_RenderCopy(g_renderer, g_bg_sheet_texture, &g_bg_source_rect, &g_destination_bg);

	// 비행기
	SDL_RenderCopy(g_renderer, g_plane_sheet_texture, &g_plane_source_rect, &g_destination_plane); // texture를 복사해서 화면에 나타내주는 함수
		
	//여기서도 for문 돌려서 flag가 true인 것만 그리기!
	DrawMissile();

	// 점수
	DrawScoreText();

	// 백에서 그린 그림을 한번에 가져옴
	SDL_RenderPresent(g_renderer); 
}

/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
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
// 프로그램이 끝날 때 한 번 호출되는 함수.
// 이 함수에서 사용된 자원(이미지, 사운드 등)과 메모리 등을 해제해야한다.
void ClearGame()
{
	SDL_DestroyTexture(g_bg_sheet_texture);
	SDL_DestroyTexture(g_plane_sheet_texture); // 비행기 메모리 해제
	SDL_DestroyTexture(g_scoreText_kr); // 한글 점수 텍스쳐 메모리 해제
	SDL_DestroyTexture(g_scoreText_num); // 한글 점수 텍스쳐 메모리 해제
	TTF_CloseFont(g_font); // 폰트 메모리 해제

	for (int i = 0; i < 5; i++)
	{
		SDL_DestroyTexture(missile_arr[i].missile_sheet_texture); // 미사일 메모리 해제
	}
	Mix_FreeMusic(g_bg_mus);
	Mix_FreeChunk(g_missile_fire_sound);
}

void MakeGameObjTextures()
{
	SDL_Surface* bg_1_sheet_surface = IMG_Load("../../Resources/back1.jpg"); // 이미지 파일을 가져옴
	g_bg_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bg_1_sheet_surface);

	SDL_Surface* plane_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // 이미지 파일을 가져옴
	SDL_SetColorKey(plane_sheet_surface, SDL_TRUE, SDL_MapRGB(plane_sheet_surface->format, 255, 255, 255));
	g_plane_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, plane_sheet_surface);

	SDL_Surface* missile_sheet_surface = IMG_Load("../../Resources/Airplane.png"); // 이미지 파일을 가져옴
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
			SDL_RenderCopy(g_renderer, missile_arr[i].missile_sheet_texture, &g_missile_source_rect, &missile_arr[i].destination_missile); // texture를 복사해서 화면에 나타내주는 함수
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
			missile_arr[i].destination_missile.y -= 50; // 미사일 위치 설정

			int score_int = std::stoi(g_score_str);
			score_int += 10;
			g_score_str = std::to_string(score_int);
			while (g_score_str.size() != 5)
				g_score_str = "0" + g_score_str; // 스코어 갱신

			Mix_PlayChannel(-1, g_missile_fire_sound, 0); //발사 효과음
			break;
		}
	}
}

void DrawScoreText()
{
	// Draw Score (Korean)
	// 화면에 표시 될 위치
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
	SDL_FreeSurface(tmp_surface); // 계속 해제해줘야함

	tmp_r.x = 85;
	tmp_r.y = 0;
	tmp_r.w = g_scoreText_num_rect.w;
	tmp_r.h = g_scoreText_num_rect.h;

	SDL_RenderCopy(g_renderer, g_scoreText_num, &g_scoreText_num_rect, &tmp_r);
	SDL_DestroyTexture(g_scoreText_num); // 계속 해제해줘야함
}
void InitBGM()
{
	g_bg_mus = Mix_LoadMUS("../../Resources/Retrocity.mp3"); // 배경음악 로드
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"Kalimba.mp3\"): " << Mix_GetError() << std::endl;
	}
	Mix_VolumeMusic(24);
}
void InitMissileChunk()
{
	g_missile_fire_sound = Mix_LoadWAV("../../Resources/fire.mp3"); // 효과음 로드
	Mix_VolumeChunk(g_missile_fire_sound, 24);
}
void InitScoreText()
{
	// scoreText_kr Title
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 24);
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(g_font, CW2A(L"점수: ", CP_UTF8), white);
	//텍스트 가져오기
	g_scoreText_kr_rect.x = 0;
	g_scoreText_kr_rect.y = 0;
	g_scoreText_kr_rect.w = tmp_surface->w;
	g_scoreText_kr_rect.h = tmp_surface->h;

	g_scoreText_kr = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);

	// scoreText_num
	g_font = TTF_OpenFont("../../Resources/MaruBuri-SemiBold.ttf", 28); // 전역변수 선언
}
