#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Events.h"

using namespace std;

class Ship
{
public:
	Ship(SDL_Texture* texture);
	~Ship();

	void Render(SDL_Renderer* renderer);
	void Update(float dt);
	void HandleEvent(Event e);
private:
	float velocityMax = 0.15;
	float posX = 0.0;
	float velocityX = 0.0;
	SDL_Texture* texture;
	SDL_Rect sourceRect;
};

