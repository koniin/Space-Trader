#include "Station.h"


Station::Station(SDL_Texture* texture, SDL_Point* startPoint, SDL_Point* worldBounds) 
	: GameObject(texture, worldBounds) {
	sourceRect.x = startPoint->x;
	sourceRect.y = startPoint->y;
	sourceRect.w = 256;
	sourceRect.h = 256;
	visited = false;
	gameObjectType = GameObject::Type::Station;
}

Station::~Station()
{
}

void Station::Render(SDL_Renderer* renderer, int cameraX, int cameraY) {
	if (IsDestroyed())
		return;

	SDL_Rect renderQuad = { sourceRect.x - cameraX, sourceRect.y - cameraY, sourceRect.w, sourceRect.h };
	SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, angle + angleAdjustment, NULL, SDL_FLIP_NONE);
}

void Station::Update(float dt) {
	angle += turnSpeed;
}

void Station::HandleEvent(Event e) {
}

void Station::Collide(const GameObject* gameObject) {
	if (gameObject->GetType() == GameObject::Type::Player) {
		Destroy();
	}
}