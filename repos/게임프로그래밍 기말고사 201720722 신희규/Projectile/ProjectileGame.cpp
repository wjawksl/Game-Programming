#include <iostream>

#include "ProjectileGame.h" 
#include "SDL_image.h"
#include "G2W.h"
#include "math.h"


extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern float g_timestep_s;
extern float error;
extern float edge_error;

float timeCheck = 0.0f;

ProjectileGame::ProjectileGame()
{
	g_flag_running = true;

	simul_ball = new Ball(0.11f, &room_);

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


	// Initialize ball pointers
	for ( int i=0; i<MAX_BALL_NUM; i++ )
	{
		balls_[i] = 0;
	}

	num_of_balls_ = 0;

	mouse_win_x_ = 0;
	mouse_win_y_ = 0;

	AddNewBall();
}



ProjectileGame::~ProjectileGame()
{
	// Delete balls
	for ( int i=0; i<MAX_BALL_NUM; i++ )
	{
		if ( balls_[i] != 0 )
			delete balls_[i];

		balls_[i] = 0;
	}

	num_of_balls_ = 0;
	SDL_DestroyTexture(ball_texture_);
}


void
ProjectileGame::AddNewBall()
{
	if ( num_of_balls_ == MAX_BALL_NUM ) return;

	// Create new Ball
	Ball *ball = new Ball(0.11f, &room_);

	// Add to the list
	balls_[num_of_balls_] = ball;

	// Increase Num
	num_of_balls_++;

}


void
ProjectileGame::Update()
{
	// Update balls
	for ( int i=0; i<num_of_balls_; i++ )
	{
		balls_[i]->Update(g_timestep_s);
	}

	timeCheck += g_timestep_s;

	if (timeCheck >= 1.0f)
	{
		timeCheck = 0.0f;

		pos_idx = 0;
		for (int i = 0; i < 70; i++)
		{
			draw_simul_ball_pos[i] = simul_ball_pos[i];
		}
		simul_ball->Reset();	
	
		Shoot(simul_ball);
	}	
	simul_ball->Update(g_timestep_s);
	simul_ball_pos[pos_idx] = { simul_ball->pos_x(), simul_ball->pos_y() };
	
	pos_idx++;
}

void 
ProjectileGame::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255,255,255,255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color
	
	
	// Draw room_
	{
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

		// Left Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.left_wall_x()),
										G2W_Y(room_.height()) );

		
		// Right Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.right_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(room_.height()) );

		// Top Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(room_.height()), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(room_.height()) );

		// Bottom Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(0) );

		// Fence
		SDL_RenderDrawLine(g_renderer, G2W_X( room_.vertiacal_fence_pos_x() ), 
										G2W_Y(0), 
										G2W_X( room_.vertiacal_fence_pos_x()  ),
										G2W_Y( room_.ground_height()+room_.vertiacal_fence_height() ) );
	}


	// Draw Balls
	for ( int i=0; i<num_of_balls_; i++ )
	{
		Ball *ball = balls_[i];

		int ball_win_x = G2W_X(balls_[i]->pos_x());
		int ball_win_y = G2W_Y(balls_[i]->pos_y());

		double win_radius = G2W_Scale * balls_[i]->radius();

		SDL_Rect dest_rect;
		dest_rect.w = (int)(2*win_radius);
		dest_rect.h = (int)(2*win_radius);
		dest_rect.x = (int)(ball_win_x - win_radius);
		dest_rect.y = (int)(ball_win_y - win_radius);

		SDL_RenderCopy(g_renderer, ball_texture_, &ball_src_rectangle_, &dest_rect);
	}

	for (int i = 0; i < 70; i++)
	{
		int simul_ball_win_x = G2W_X(draw_simul_ball_pos[i].x);
		int simul_ball_win_y = G2W_Y(draw_simul_ball_pos[i].y);

		double simul_win_radius = G2W_Scale * simul_ball->radius();

		SDL_Rect simul_dest_rect;
		simul_dest_rect.w = (int)(2 * simul_win_radius);
		simul_dest_rect.h = (int)(2 * simul_win_radius);
		simul_dest_rect.x = (int)(simul_ball_win_x - simul_win_radius);
		simul_dest_rect.y = (int)(simul_ball_win_y - simul_win_radius);

		SDL_RenderDrawPoint(g_renderer, simul_ball_win_x, simul_ball_win_y);
	}
	
	
	// Draw the Guide Line 
	if ( num_of_balls_ > 0 )
	{
		SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 100);
		SDL_RenderDrawLine(g_renderer, G2W_X(balls_[num_of_balls_-1]->pos_x()), 
										G2W_Y(balls_[num_of_balls_-1]->pos_y()), 
										mouse_win_x_, 
										mouse_win_y_ );
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void 
ProjectileGame::HandleEvents()
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
				// Get the cursor's x position.
				mouse_win_x_ = event.button.x;
				mouse_win_y_ = event.button.y;			
							
				// Launch
				if ( num_of_balls_ > 0 )
				{
					Ball *ball = balls_[num_of_balls_-1];

					Shoot(ball);
					for (int i = 0; i < 70; i++)
					{
						draw_simul_ball_pos[i] = {0,0};
					}
					// Add a new ball for the next
					AddNewBall();
				}
			}

		case SDL_MOUSEMOTION:
			{
				// Get the cursor's x position.
				mouse_win_x_ = event.button.x;
				mouse_win_y_ = event.button.y;						
			}
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_1)
			{
				G2W_Scale = 300.0f;
				W2G_Scale = 1.f / 300.0f;			
			}
			if (event.key.keysym.sym == SDLK_2)
			{
				G2W_Scale = 150.0f;
				W2G_Scale = 1.f / 150.0f;	
			}
		default:
			break;
		}
	}
}
void
ProjectileGame::Shoot(Ball *ball)
{
	mouse_game_x = W2G_X(mouse_win_x_);
	mouse_game_y = W2G_Y(mouse_win_y_);

	// Guide Line Vector
	guide_line_x = mouse_game_x - ball->pos_x();
	guide_line_y = mouse_game_y - ball->pos_y();

	// Lauching Force
	launcing_force_x = 8.0 * guide_line_x;
	launcing_force_y = 8.0 * guide_line_y;

	// Launch
	ball->Launch(launcing_force_x, launcing_force_y);
}
