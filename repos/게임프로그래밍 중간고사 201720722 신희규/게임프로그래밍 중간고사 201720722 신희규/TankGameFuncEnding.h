#pragma once
#include "GameFunc.h"

class Ending : public PhaseInterface
{
public:
	Ending();
	virtual ~Ending();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	SDL_Texture* texture_ending_; // the SDL_Texture 
	SDL_Rect source_rectangle_ending_; // the rectangle for source image
	SDL_Rect destination_rectangle_ending_; // for destination

	SDL_Texture* g_game_end_text_kr;
	SDL_Rect g_game_end_text_kr_rect;
	TTF_Font* g_font_end; // 게임 폰트 선언

	SDL_Color black = { 0,0,0,0 };
};

