#include "Ship.h"
#include <stdio.h>
#include <algorithm>
#include <math.h> 

Ship::Ship(SDL_Texture* tex) {
	texture = tex;
	posX = 100.0f;
	posY = 100.0f;
	sourceRect.x = posX;
	sourceRect.y = posY;
	sourceRect.w = 50;
	sourceRect.h = 19;
	speed = 0;
	precalc_speed = 0.0f;
	speedForwardMax = 15;
	speedBackwardMax = -10;
	angle = 90.0f;
	angleAdjustment = -90;
}

Ship::~Ship()
{
}

void Ship::Render(SDL_Renderer* renderer) {
	SDL_RenderCopyEx(renderer, texture, NULL, &sourceRect, angle - 90, NULL, SDL_FLIP_NONE);
}

void Ship::HandleEvent(Event e) {
	if (e == ACCELERATE) {
		speed += 5;
		setSpeed(std::min(speed, speedForwardMax));
	}
	if (e == DECELERATE) {
		speed -= 5;
		setSpeed(std::max(speed, speedBackwardMax));
	}
	if (e == MOVE_RIGHT) {
		angle += 5;
	}
	if (e == MOVE_LEFT) {
		angle -= 5;
	}
}

// Handle velocity different, but max and min velocity in update and handle constant deceleration?? 
// or maybe just keep it as "gears" as it is, in constant increase/decrease no need to hold keys to keep it less arcadey

float clamp(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

void Ship::Update(float dt) {
	posX += cos((angleAdjustment + angle) * M_PI / 180) * (precalc_speed * dt);
	posY += sin((angleAdjustment + angle) * M_PI / 180) * (precalc_speed * dt);
	sourceRect.x = posX;
	sourceRect.y = posY;
	
	float clamped = std::max(0, std::min(sourceRect.x, 640 - sourceRect.w));
	if (sourceRect.x != (int)clamped) {
		setSpeed(0);
		sourceRect.x = clamped;
	}
	clamped = std::max(0, std::min(sourceRect.y, 480 - sourceRect.h));
	if (sourceRect.y != (int)clamped) {
		setSpeed(0);
		sourceRect.y = clamped;
	}
}

void Ship::setSpeed(int newSpeed) {
	speed = newSpeed;
	precalc_speed = speed == 0 ? 0.0f : (speed / 100.f);
}