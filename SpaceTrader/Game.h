#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <memory>

using namespace std;

struct _SurfaceDeleter { void operator()(SDL_Surface* tex) const { SDL_FreeSurface(tex); printf("surface deleted. \n"); } };
using SurfacePtr = std::unique_ptr<SDL_Surface, _SurfaceDeleter>;

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
	//The surface contained by the window
	SDL_Surface* screenSurface;
	// Background
	SurfacePtr background;

	bool Init();
	void GameLoop();
	void HandleInput();
	void Render();
	SurfacePtr LoadSurface(std::string path);
};

