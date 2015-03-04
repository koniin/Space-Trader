#include "Game.h"

using namespace std;

Game::Game() {
	printf("Starting game \n");
}

Game::~Game() {
}

void Game::Run() {
	if (Init())
		GameLoop();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Game::Init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Init SDL_Image with PNG support
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}
	
	//Get window surface
	screenSurface = SDL_GetWindowSurface(window);
	background = LoadSurface("2.png");
	
	return true;
}

void Game::GameLoop() {
	bool quit = false;

	//Event handler
	SDL_Event e;

	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
					case SDLK_1:
						background = LoadSurface("3.png");
						break;
					case SDLK_2:
						background = LoadSurface("2.png");
						break;
					case SDLK_q:
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
			}
		}
		
		Render();
	}
}

void Game::Render() {
	//Apply the PNG image
	SDL_BlitSurface(background.get(), NULL, screenSurface, NULL);
	//Update the surface
	SDL_UpdateWindowSurface(window);
}

SurfacePtr Game::LoadSurface(std::string path) {
	SurfacePtr optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		//Convert surface to screen format
		optimizedSurface = SurfacePtr(SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL));
		if (optimizedSurface == NULL) {
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}