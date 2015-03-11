#include "Station.h"


Station::Station(SDL_Texture* texture, SDL_Point* startPoint, SDL_Point* worldBounds) 
	: GameObject(texture, worldBounds) {
}


Station::~Station()
{
}

void Station::Render(SDL_Renderer* renderer, int cameraX, int cameraY) {

}

void Station::Update(float dt) {

}

void Station::HandleEvent(Event e) {

}
