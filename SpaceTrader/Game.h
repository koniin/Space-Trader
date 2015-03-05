#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <memory>

using namespace std;

struct _TextureDeleter { void operator()(SDL_Texture* tex) const { SDL_DestroyTexture(tex); printf("texture deleted. \n"); } };
using TexturePtr = std::unique_ptr<SDL_Texture, _TextureDeleter>;

class Game
{
public:
	Game();
	~Game();
	void Run();

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const char* TITLE = "Space Trader";
	bool quit = false;
	//The window we'll be rendering to
	SDL_Window* window;
	// Background
	TexturePtr background;
	
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;

	bool Init();
	void GameLoop();
	void HandleInput();
	void Render();
	TexturePtr LoadTexture(string path);
};

