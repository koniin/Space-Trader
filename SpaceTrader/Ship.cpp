#include "Ship.h"
#include <stdio.h>
#include <algorithm>
#include <math.h> 

Ship::Ship(SDL_Texture* tex, SDL_Point* startPoint, SDL_Point* world) 
	: GameObject(tex, world) {
	posX = startPoint->x;
	posY = startPoint->y;
	sourceRect.x = startPoint->x;
	sourceRect.y = startPoint->y;
	sourceRect.w = 50;
	sourceRect.h = 19;
	speed = 0;
	precalc_speed = 0.0f;
	speedForwardMax = 15;
	speedBackwardMax = -10;
	angle = 90.0f;
	angleAdjustment = -90;
	startPoint = NULL;
	gameObjectType = GameObject::Type::Player;
}

void Ship::Render(SDL_Renderer* renderer, int cameraX, int cameraY) {
	SDL_Rect renderQuad = { sourceRect.x - cameraX, sourceRect.y - cameraY, sourceRect.w, sourceRect.h };
	SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, angle + angleAdjustment, NULL, SDL_FLIP_NONE);
}

void Ship::HandleEvent(Event e) {
	if (e == ACCELERATE) {
		speed += speedIncreaseStep;
		SetSpeed(std::min(speed, speedForwardMax));
	}
	if (e == DECELERATE) {
		speed -= speedIncreaseStep;
		SetSpeed(std::max(speed, speedBackwardMax));
	}
	if (e == MOVE_RIGHT) {
		angle += turnSpeed;
	}
	if (e == MOVE_LEFT) {
		angle -= turnSpeed;
	}
}

void Ship::Collide(const GameObject* gameObject) {
	if (gameObject->GetType() == GameObject::Type::Station)
		cargo++;
}

// Handle velocity different, but max and min velocity in update and handle constant deceleration?? 
// or maybe just keep it as "gears" as it is, in constant increase/decrease no need to hold keys to keep it less arcadey

void Ship::Update(float dt) {	
	posX += cos((angleAdjustment + angle) * M_PI / 180) * (precalc_speed * dt);
	posY += sin((angleAdjustment + angle) * M_PI / 180) * (precalc_speed * dt);
	sourceRect.x = posX;
	sourceRect.y = posY;

	KeepInBounds();
}

void Ship::KeepInBounds() {
	float clamped = std::max(0, std::min(sourceRect.x, worldBounds->x - sourceRect.w));
	if (sourceRect.x != (int)clamped) {
		SetSpeed(0);
		sourceRect.x = clamped;
	}
	clamped = std::max(0, std::min(sourceRect.y, worldBounds->y - sourceRect.h));
	if (sourceRect.y != (int)clamped) {
		SetSpeed(0);
		sourceRect.y = clamped;
	}
}

const int Ship::GetCargo() {
	return cargo;
}