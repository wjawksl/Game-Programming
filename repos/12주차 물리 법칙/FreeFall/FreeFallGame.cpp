#include <iostream>

#include "FreeFallGame.h" 
#include "SDL_image.h"
#include "G2W.h"
#include "math.h"


extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern float g_timestep_s;

FreeFallGame::FreeFallGame()
{
	g_flag_running = true;


	// Texture
	{
		SDL_Surface* ball_surface = IMG_Load("../../Resources/ball.png");
		ball_src_rectangle_.x = 0;
		ball_src_rectangle_.y = 0;
		ball_src_rectangle_.w = ball_surface->w;
		ball_src_rectangle_.h = ball_surface->h;

		ball_texture_ = SDL_CreateTextureFromSurface(g_renderer, ball_surface);
		SDL_FreeSurface(ball_surface);
	}

	// ���� ������
	ball_radius_ = 0.11f;

	// ���� ���� ��ġ
	ball_pos_[0] = 0.f;
	ball_pos_[1] = 2.f;
	
	// ���� ���� �ӵ�
	ball_vel_[0] = 0.f;
	ball_vel_[1] = 0.f;


	mouse_win_x_ = 0;
	mouse_win_y_ = 0;

	start_ = false;
}



FreeFallGame::~FreeFallGame()
{
	SDL_DestroyTexture(ball_texture_);
}



void
FreeFallGame::Update()
{
	if (start_) {

		// �߷� ���ӵ�
		float g[2];
		g[0] = 0.f;
		g[1] = -9.8f;

		// g_timestep_s �ð��� ���� �� ���� ��ġ ���
		ball_pos_[0] = ball_pos_[0] + g_timestep_s * ball_vel_[0];
		ball_pos_[1] = ball_pos_[1] + g_timestep_s * ball_vel_[1];

		// ���� �������� ���̶��, �ٴڰ� �浹�Ͽ����� �˻�
		if (ball_vel_[1] < 0.f && ball_pos_[1] - ball_radius_ <= 0.f )
		{

			// ��ġ ����.
			// ���� ���� �ٴ��� �հ� ������ ���� ���ɼ��� ũ�Ƿ�, 
			// �ٴڿ� ���ϴ� ��ġ�� �ű��.
			ball_pos_[1] = 0.f + ball_radius_;

			// �浹 ���� ���
			// �ٴ��� �����ϹǷ� ���� ���⿡ ���ؼ��� �ӵ��� ������ ��ȭ ��Ų��.
			ball_vel_[1] = -1 * ball_vel_[1] * 0.7f; // ź�� ��� 0.7
		}

		// g_timestep_s �ð��� ���� �� ���� �ӵ� ���
		ball_vel_[0] = ball_vel_[0] + g_timestep_s * g[0];
		ball_vel_[1] = ball_vel_[1] + g_timestep_s * g[1];


		std::cout << " v: " << ball_vel_[1];
		std::cout << " p: " << ball_pos_[1];
		std::cout << std::endl;
	}

}

void 
FreeFallGame::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255,255,255,255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color
	
	// Ball
	SDL_Rect dest_rect;
	dest_rect.w = (int)(2* ball_radius_ * G2W_Scale);
	dest_rect.h = (int)(2* ball_radius_ * G2W_Scale);
	dest_rect.x = G2W_X(ball_pos_[0]) - (G2W_Scale*ball_radius_);
	dest_rect.y = G2W_Y(ball_pos_[1]) - (G2W_Scale*ball_radius_);
	
	SDL_RenderCopy(g_renderer, ball_texture_, &ball_src_rectangle_, &dest_rect);

	// Ground Line
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(g_renderer, G2W_X(-1.5f),
		G2W_Y(0),
		G2W_X(1.5f),
		G2W_Y(0));

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void 
FreeFallGame::HandleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
		
			// If the mouse left button is pressed. 
			if ( event.button.button == SDL_BUTTON_LEFT )
			{
				start_ = true;
				// Get the cursor's x position.
				/*mouse_win_x_ = event.button.x;
				mouse_win_y_ = event.button.y;

				
				double mouse_game_x = W2G_X(mouse_win_x_);
				double mouse_game_y = W2G_Y(mouse_win_y_);*/
				
			}

		

		case SDL_MOUSEMOTION:
			{
				// Get the cursor's x position.
				//mouse_win_x_ = event.button.x;
				//mouse_win_y_ = event.button.y;

				
				
			}
			break;

		default:
			break;
		}
	}
}
