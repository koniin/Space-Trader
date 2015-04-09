#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <vector>
#include "StateManager.h"
#include "GameObject.h"
#include "Ship.h"
#include "Statistics.h"

using namespace std;


class Game
{
public:
	Game();
	~Game();
	void Run();

private:
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 360;
	const int LEVEL_WIDTH = 2048;
	const int LEVEL_HEIGHT = 2048;
	const char* TITLE = "Space Trader";
	const unsigned int TICKS_PER_SECOND = 50;
	const unsigned int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const unsigned int MAX_FRAMESKIP = 10;
	
	StateManager stateManager;
	SDL_Point worldBounds;

	unique_ptr<Statistics> statistics;
	
	std::map<SDL_Keycode, Event> keysDown;

	Uint32 fps_lasttime; //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames; //frames passed since the last recorded fps.

	bool quit = false;
	//The window we'll be rendering to
	SDL_Window* window;
	
	SDL_Renderer* renderer = NULL;

	bool InitSDL();
	bool Load();
	void GameLoop();
	void HandleInput();
};

