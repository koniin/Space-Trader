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
	int speedForwardMax;
	int speedBackwardMax;
	int direction;
	int speed;
	float precalc_speed;
	float posX;
	SDL_Texture* texture;
	SDL_Rect sourceRect;
};

