#include "Ship.h"
#include <stdio.h>
#include <algorithm>

Ship::Ship(SDL_Texture* tex) {
	texture = tex;
	sourceRect.x = 0;
	sourceRect.y = 100;
	sourceRect.w = 50;
	sourceRect.h = 19;
	speed = 0;
	precalc_speed = 0.0f;
	speedForwardMax = 15;
	speedBackwardMax = 10;
	direction = 0;
	posX = 0.0f;
}

Ship::~Ship()
{
}

void Ship::Render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, texture, NULL, &sourceRect);
}

void Ship::HandleEvent(Event e) {
	if (e == MOVE_RIGHT) {
		if (speed > 0 && direction == -1) {
			speed -= 5;
		} else {
			speed += 5;
			direction = 1;
		}
		speed = std::min(speed, speedForwardMax);
	}
	if (e == MOVE_LEFT) {
		if(speed > 0 && direction == 1) {
			speed -= 5;
		} else {
			speed += 5;
			direction = -1;
		}
		speed = std::min(speed, speedBackwardMax);
	}
	precalc_speed = (speed / 100.f);
}

// Handle velocity different, but max and min velocity in update and handle constant deceleration?? 
// or maybe just keep it as "gears" as it is, in constant increase/decrease no need to hold keys to keep it less arcadey

void Ship::Update(float dt) {
	posX += dt * precalc_speed * direction;
	sourceRect.x = posX;

	int xMax = 640 - sourceRect.w;
	if (sourceRect.x > xMax)
		sourceRect.x = xMax;
	if (sourceRect.x < 0)
		sourceRect.x = 0;
}
