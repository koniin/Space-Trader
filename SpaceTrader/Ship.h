#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include "Events.h"

using namespace std;

class Ship
{
public:
	Ship(SDL_Texture* texture, SDL_Point* startPoint, shared_ptr<SDL_Point> worldBounds);
	~Ship();

	void Render(SDL_Renderer* renderer);
	void Update(float dt);
	void HandleEvent(Event e);
private:
	shared_ptr<SDL_Point> worldBounds;
	int speedForwardMax;
	int speedBackwardMax;
	int directionX;
	int directionY;
	int speed;
	int angleAdjustment;
	float precalc_speed;
	float posX;
	float posY;
	float turnSpeed = 0.02f;
	int speedIncreaseStep = 5;
	double angle;
	SDL_Texture* texture;
	SDL_Rect sourceRect;

	void KeepInBounds();
	void SetSpeed(int newSpeed);
};

