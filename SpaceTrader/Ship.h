#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <string>
#include "Events.h"
#include "GameObject.h"

class Ship : public GameObject
{
public:
	Ship(SDL_Texture* texture, SDL_Point* startPoint, SDL_Point* worldBounds);

	void Render(SDL_Renderer* renderer, int cameraX, int cameraY);
	void Update(float dt);
	void HandleEvent(Event e);
	void Collide(const GameObject* gameObject);
	const int GetCargo() const;
	const int GetResources() const;
private:
	int speedForwardMax;
	int speedBackwardMax;
	int directionX;
	int directionY;
	int cargo = 0;
	int resources = 0;
	float posX;
	float posY;
	float speed;
	const float turnSpeed;
	const int speedIncreaseStep = 5;

	void KeepInBounds();

	float friction;
	float velocityX;
	float velocityY;
};

