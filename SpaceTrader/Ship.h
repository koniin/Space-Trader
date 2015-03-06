#pragma once
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class Ship
{
public:
	Ship(SDL_Texture* texture);
	~Ship();

	void Render(SDL_Renderer* renderer);
	void Update(float dt);
private:
	SDL_Texture* texture;
	SDL_Rect sourceRect;
};

