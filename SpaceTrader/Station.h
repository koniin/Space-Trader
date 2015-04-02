#pragma once

#include "GameObject.h"

class Station : public GameObject
{
public:
	Station(SDL_Texture* texture, SDL_Point* startPoint, SDL_Point* worldBounds);
	~Station();

	void Render(SDL_Renderer* renderer, int cameraX, int cameraY);
	void Update(float dt);
	void HandleEvent(Event e);
	void Collide(const GameObject* gameObject);
private:
	const float turnSpeed = 0.0005f;
};

