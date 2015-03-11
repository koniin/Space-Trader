#include "Station.h"


Station::Station(SDL_Texture* texture, SDL_Point* startPoint, SDL_Point* worldBounds) 
	: GameObject(texture, worldBounds) {
	sourceRect.x = startPoint->x;
	sourceRect.y = startPoint->x;
	sourceRect.w = 256;
	sourceRect.h = 256;
}

Station::~Station()
{
}

void Station::Render(SDL_Renderer* renderer, int cameraX, int cameraY) {
	SDL_Rect renderQuad = { sourceRect.x - cameraX, sourceRect.y - cameraY, sourceRect.w, sourceRect.h };
	SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, angle + angleAdjustment, NULL, SDL_FLIP_NONE);
}

void Station::Update(float dt) {
	angle += turnSpeed;
}

void Station::HandleEvent(Event e) {
}
