#pragma once

#include "GameFunc.h"
#include "SDL_mixer.h"

class Intro : public PhaseInterface
{
public:
	Intro();
	virtual ~Intro();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	Mix_Music* g_intro_mus; // 인트로 배경음악 변수 선언

	SDL_Texture* texture_intro_; // the SDL_Texture 
	SDL_Rect source_rectangle_intro_; // the rectangle for source image
	SDL_Rect destination_rectangle_intro_; // for destination

	SDL_Texture* g_game_start_text_kr;
	SDL_Rect g_game_start_text_kr_rect;
	TTF_Font* g_font_intro; // 게임 폰트 선언

	SDL_Color black = { 0,0,0,0 };
};