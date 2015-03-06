#include "Ship.h"
#include <stdio.h>

Ship::Ship(SDL_Texture* tex) {
	texture = tex;
	sourceRect.x = 100;
	sourceRect.y = 100;
	sourceRect.w = 50;
	sourceRect.h = 50;
}


Ship::~Ship()
{
}

void Ship::Render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, NULL, &sourceRect);
}

float posX = 0.0;

void Ship::Update(float dt) {
	posX += dt * 0.3;
	sourceRect.x = posX;
	if (sourceRect.x > 590)
		sourceRect.x = 590;
}
