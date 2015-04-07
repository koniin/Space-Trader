#include "Station.h"
#include <random>

Station::Station(SDL_Texture* texture, SDL_Point* startPoint, SDL_Point* worldBounds) 
	: GameObject(texture, worldBounds) {
	sourceRect.x = startPoint->x;
	sourceRect.y = startPoint->y;
	sourceRect.w = 16;
	sourceRect.h = 16;
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
	//angle += turnSpeed;
}

void Station::HandleEvent(Event e) {
}

void Station::Collide(const GameObject* gameObject) {
	if (gameObject->GetType() == GameObject::Type::Player) {
		
		//the random device that will seed the generator
		std::random_device seeder;
		//then make a mersenne twister engine
		std::mt19937 engine(seeder());
		//then the easy part... the distribution
		std::uniform_int_distribution<int> randX(sourceRect.w, worldBounds->x - sourceRect.w);
		std::uniform_int_distribution<int> randY(sourceRect.h, worldBounds->y - sourceRect.h);
		//then just generate the integer like this:
		sourceRect.x = randX(engine);
		sourceRect.y = randY(engine);

		printf("%i - ", sourceRect.x);
		printf("%i", sourceRect.y);
	}
}