#include "Ship.h"
#include <stdio.h>
#include <algorithm>
#include <math.h> 

#define _USE_MATH_DEFINES
#include <cmath>

Ship::Ship(SDL_Texture* tex, SDL_Point* startPoint, SDL_Point* world) 
	: GameObject(tex, world),
	friction(0.98f),
	velocityX(0.0f),
	velocityY(0.0f),
	speed(0.011f),
	turnSpeed(0.002f) {
	posX = startPoint->x;
	posY = startPoint->y;
	sourceRect.x = startPoint->x;
	sourceRect.y = startPoint->y;
	sourceRect.w = 50;
	sourceRect.h = 19;
	precalc_speed = 0.0f;
	speedForwardMax = 15;
	speedBackwardMax = -10;
	angle = 0.0f;
	angleAdjustment = -90;
	startPoint = NULL;
	gameObjectType = GameObject::Type::Player;
}

void Ship::Render(SDL_Renderer* renderer, int cameraX, int cameraY) {
	SDL_Rect renderQuad = { sourceRect.x - cameraX, sourceRect.y - cameraY, sourceRect.w, sourceRect.h };
	SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, angle + angleAdjustment, NULL, SDL_FLIP_NONE);
}

void Ship::HandleEvent(Event e) {
	float radians = angle / M_PI * 180;
	if (e == ACCELERATE) {
		velocityX += std::cos(radians) * speed;
		velocityY += std::sin(radians) * speed;
		
		//velocity.x = std::min(velocity.x, speedForwardMax);
		//velocity.y = std::min(velocity.y, speedForwardMax);
	}
	/*
	if (e == DECELERATE) {
		speed -= speedIncreaseStep;
		SetSpeed(std::max(speed, speedBackwardMax));
	}
	if (e == MOVE_RIGHT) {
		angle += turnSpeed;
	}*/
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

// Handle velocity different, but max and min velocity in update and handle constant deceleration?? 
// or maybe just keep it as "gears" as it is, in constant increase/decrease no need to hold keys to keep it less arcadey

void Ship::Update(float dt) {	
	

	//this.px = this.position.x + 20 * std::cos(radians);
	//this.py = this.position.y + 20 * std::sin(radians);

	// Apply friction
	velocityX *= friction;
	velocityY *= friction;

	// Apply the velocity to the position
	posX += (velocityX * dt);
	posY += (velocityY * dt);

	sourceRect.x = posX;
	sourceRect.y = posY;
	/* 
	posX += cos((angleAdjustment + angle) * M_PI / 180) * (precalc_speed * dt);
	posY += sin((angleAdjustment + angle) * M_PI / 180) * (precalc_speed * dt);
	sourceRect.x = posX;
	sourceRect.y = posY;
	*/
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

const int Ship::GetCargo() const {
	return cargo;
}

const int Ship::GetResources() const {
	return resources;
}