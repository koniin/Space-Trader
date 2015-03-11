#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include "GameObject.h"

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
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int LEVEL_WIDTH = 2048;
	const int LEVEL_HEIGHT = 2048;
	const char* TITLE = "Space Trader";

	const unsigned int TICKS_PER_SECOND = 50;
	const unsigned int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const unsigned int MAX_FRAMESKIP = 10;
	
	SDL_Rect camera;
	SDL_Point worldBounds;

	unique_ptr<GameObject> ship;
	unique_ptr<GameObject> station;
	
	std::map<SDL_Keycode, Event> keysDown;

	Uint32 fps_lasttime; //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames; //frames passed since the last recorded fps.

	bool quit = false;
	//The window we'll be rendering to
	SDL_Window* window;
	TexturePtr backgroundLayers[2];
	TexturePtr shipTexture;
	TexturePtr stationTexture;

	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;

	bool InitSDL();
	bool Load();
	void GameLoop();
	void GameLoop2();
	void GameLoop3();
	void HandleInput();
	void Update(float dt);
	void UpdateCamera();
	void Render();
	TexturePtr LoadTexture(string path);
};

