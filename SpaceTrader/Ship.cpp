#include "Ship.h"
#include <stdio.h>
#include <algorithm>
#include <math.h> 

#define _USE_MATH_DEFINES
#include <cmath>

Ship::Ship(SDL_Texture* tex, SDL_Point* startPoint, SDL_Point* world) 
	: GameObject(tex, world),
	friction(0.02f),
	velocityX(0.0f),
	velocityY(0.0f),
	turnSpeed(4.0f) {
	posX = startPoint->x;
	posY = startPoint->y;
	sourceRect.x = startPoint->x;
	sourceRect.y = startPoint->y;
	sourceRect.w = 50;
	sourceRect.h = 19;
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
		// Create a normalized vector in the direction of travel
		float directionX = static_cast<float>(sin(2 * M_PI * (angle / 360)));
		float directionY = static_cast<float>(cos(2 * M_PI * (angle / 360)));

		// Add to velocity vector (using minus for y because 0,0 is the top-left corner instead of bottom-left)
		velocityX += directionX * 0.01f;
		velocityY -= directionY * 0.01f;
	}
	if (e == DECELERATE) {
		velocityX *= 0.9;
		velocityY *= 0.9;
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
		if (cargo > 0) {
			cargo = 1;
			resources += 100;
		} else 
			cargo++;
}

void Ship::Update(float dt) {	
	posX += velocityX * dt;
	posY += velocityY * dt;

	// friction (in space lol)
	velocityX = velocityX - velocityX * (friction);
	velocityY = velocityY - velocityY * (friction);

	sourceRect.x = posX;
	sourceRect.y = posY;

	KeepInBounds();
}

void Ship::KeepInBounds() {
	float clamped = std::max(0, std::min(sourceRect.x, worldBounds->x - sourceRect.w));
	if (sourceRect.x != (int)clamped) {
		sourceRect.x = clamped;
	}
	clamped = std::max(0, std::min(sourceRect.y, worldBounds->y - sourceRect.h));
	if (sourceRect.y != (int)clamped) {
		sourceRect.y = clamped;
	}
}

const int Ship::GetCargo() const {
	return cargo;
}

const int Ship::GetResources() const {
	return resources;
}