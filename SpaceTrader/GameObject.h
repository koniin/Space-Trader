#pragma once

#include "SDL.h"
#include <memory>
#include "Events.h"

class GameObject
{
public:
	GameObject(SDL_Texture* tex, SDL_Point* world);
	virtual ~GameObject();
	virtual void HandleEvent(Event e) = 0;
	virtual void Update(float dt);
	virtual void Render(SDL_Renderer* renderer, int cameraX, int cameraY) = 0;
	SDL_Rect* GetPositionRectangle();

protected:
	void SetSpeed(int newSpeed);

	SDL_Point* worldBounds;
	int speed = 0;
	float precalc_speed;
	SDL_Texture* texture;
	SDL_Rect sourceRect;
};

