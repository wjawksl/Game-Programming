#pragma once

#include "SDL.h"
#include "Room.h"
#include "Ball.h"

#define MAX_BALL_NUM 1000

class ProjectileGame
{
public:
	ProjectileGame();
	~ProjectileGame();
	void Render();	
	void Update();
	void HandleEvents();

protected:
	void AddNewBall();
	void Shoot(Ball *ball);

protected:

	struct Pos {
		double x;
		double y;
	};
	// Room
	Room room_;

	// Balls
	int num_of_balls_;
	Ball *balls_[MAX_BALL_NUM];

	double guide_line_x;
	double guide_line_y;
	double launcing_force_x;
	double launcing_force_y;
	double mouse_game_x;
	double mouse_game_y;

	// Simualted Ball
	Ball* simul_ball;
	Pos simul_ball_pos[70];
	Pos draw_simul_ball_pos[70];
	int pos_idx = 0;
	// Ball Texture
	SDL_Texture* ball_texture_; 
	SDL_Rect ball_src_rectangle_;

	// Mouse
	int mouse_win_x_;
	int mouse_win_y_;

	
};