#pragma once

#include "SDL.h"
#include <memory>
#include "Events.h"

class GameObject
{
public:
	enum Type {
		NotSet,
		Player,
		Station
	};

	GameObject(SDL_Texture* tex, SDL_Point* world);
	virtual ~GameObject();
	virtual void HandleEvent(Event e) = 0;
	virtual void Update(float dt);
	virtual void Render(SDL_Renderer* renderer, int cameraX, int cameraY) = 0;
	virtual void Collide(const GameObject* gameObject) = 0;
	virtual void Destroy();
	virtual bool IsDestroyed();
	virtual Type GetType() const;
	const SDL_Rect* GetPositionRectangle();

protected:
	void SetSpeed(int newSpeed);

	SDL_Point* worldBounds;
	int angleAdjustment;
	double angle;
	int speed = 0;
	float precalc_speed;
	SDL_Texture* texture;
	SDL_Rect sourceRect;
	bool destroyed = false;
	Type gameObjectType = NotSet;
};

