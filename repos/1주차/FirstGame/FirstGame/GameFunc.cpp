#include "GameFunc.h"
#include <windows.h>

int f_input;
int m_input;
std::string f_output;
std::string m_output;

int f_X;
int f_Y;
int m_X;
int m_Y;
// 흘러간 시간 기록
double f_elapsed_time_ms;


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
void InitGame() {
	f_input = 0;
	f_output = "*";
	f_flag_running = true;

	m_input = 0;
	m_output = "!";
	m_flag_running = false;

	f_X = 15;
	f_Y = 16;
	m_X = f_X;
	m_Y = f_Y - 1;

	f_elapsed_time_ms = 0;

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
	if (f_input == 1) {
		f_X--;
	}
	else if (f_input == 2) {
		f_X++;
	}
	else if (f_input == 3) {
		f_Y--;
	}
	else if (f_input == 4) {
		f_Y++;
	}
	
	if (f_X > 29) { // 이동 범위 처리
		f_X = 29;
	}
	if (f_X < 0) {
		f_X = 0;
	}
	if (f_Y > 19) {
		f_Y = 19;
	}
	if (f_Y < 0) {
		f_Y = 0;
	}
	if (m_flag_running) {
		m_Y--;
		if (m_Y < 0) {
			m_flag_running = false;			
		}
	}
	f_elapsed_time_ms += 33;
}


/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Render() {
	//// 1. 배경 그리기.
	// 1.1. 커서를 콘솔 화면의 왼쪽 위 모서리 부분으로 옮긴다. 좌표(0, 0)
	// <windows.h>에서 제공하는 함수를 사용한다.
	COORD flight, missile;
	flight.X = 0;
	flight.Y = 0;
	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), flight);

	missile.X = 0;
	missile.Y = 0;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), missile);
	// 1.2. 배경 부분을 '.'으로 채운다.
	for ( int i=0; i<20; i++ ) {
		for ( int j=0; j<30; j++ ) {
			std::cout << "_";
		}
		std::cout << std::endl;
	}

	// 1.3. 배경 아래에 시간을 표시한다,
	std::cout << "Elapsed Time: " << f_elapsed_time_ms / 1000.0f << std::endl;


	//// 2. 캐릭터 그리기.
	// 2.1. 커서를 캐릭터가 그려질 위치로 옮긴다. 
	flight.X = f_X;
	flight.Y = f_Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), flight);

	// 2.2. 비행기를 그린다.
	std::cout << f_output;

	if (m_flag_running) {
		missile.X = m_X;
		missile.Y = m_Y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), missile);

		// 2.2. 미사일을 그린다.
		std::cout << m_output;
	}
}



/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void HandleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			f_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT) {
				f_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				f_input = 2;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				f_input = 3;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				f_input = 4;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				if (!m_flag_running) {
					m_flag_running = true;
					m_X = f_X;
					m_Y = f_Y - 1;
				}
			}
			break;

		case SDL_KEYUP:
			f_input = 0;
			break;

		/*case SDL_MOUSEBUTTONDOWN:
		
			// 마우스 왼쪽 버튼이 눌려 졌을 때.
			if ( event.button.button == SDL_BUTTON_LEFT ) {
				f_input = 3;
			}
			break;*/

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
}

