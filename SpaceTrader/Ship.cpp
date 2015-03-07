#include "Ship.h"
#include <stdio.h>
#include <algorithm>

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

void Ship::HandleEvent(Event e) {
	if (e == MOVE_RIGHT) {
		velocityX += 0.05;
		velocityX = std::min(velocityX, velocityMax);
	}
	if (e == MOVE_LEFT) {
		velocityX -= 0.05;
		velocityX = std::max(velocityX, -velocityMax);
	}
}

// Handle velocity different, but max and min velocity in update and handle constant deceleration?? 
// or maybe just keep it as "gears" as it is, in constant increase/decrease no need to hold keys to keep it less arcadey

void Ship::Update(float dt) {
	posX += dt * velocityX;
	sourceRect.x = posX;

	int xMax = 640 - sourceRect.w;
	if (sourceRect.x > xMax)
		sourceRect.x = xMax;
	if (sourceRect.x < 0)
		sourceRect.x = 0;
}
