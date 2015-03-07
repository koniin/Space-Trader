#include "Ship.h"
#include <stdio.h>

Ship::Ship(SDL_Texture* tex) {
	texture = tex;
	sourceRect.x = 0;
	sourceRect.y = 100;
	sourceRect.w = 50;
	sourceRect.h = 19;
}

Ship::~Ship()
{
}

void Ship::Render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, NULL, &sourceRect);
}

float maxVelocity = 0.15;
float posX = 0.0;

void Ship::Update(float dt) {
	posX += dt * maxVelocity;
	sourceRect.x = posX;

	int xMax = 640 - sourceRect.w;
	if (sourceRect.x > xMax)
		sourceRect.x = xMax;
	if (sourceRect.x < 0)
		sourceRect.x = 0;
}
