#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "Events.h"
#include "GameObject.h"

class Ship : public GameObject
{
public:
	Ship(SDL_Texture* texture, SDL_Point* startPoint, SDL_Point* worldBounds);

	void Render(SDL_Renderer* renderer, int cameraX, int cameraY);
	void Update(float dt);
	void HandleEvent(Event e);
private:
	int speedForwardMax;
	int speedBackwardMax;
	int directionX;
	int directionY;
	int angleAdjustment;
	float posX;
	float posY;
	const float turnSpeed = 0.02f;
	const int speedIncreaseStep = 5;
	double angle;

	void KeepInBounds();
};

