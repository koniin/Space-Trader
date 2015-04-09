#include "Game.h"
#include "Ship.h"
#include "Station.h"

Game::Game() 
	: keysDown(),
	stateManager(World()) {
	printf("Starting game \n");
}

Game::~Game() {
}

void Game::Run() {
	if (InitSDL()) {
		Load();
		stateManager.SetRenderer(renderer);
		GameLoop();
	} 

	SDL_DestroyRenderer(renderer);  // slow ?
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Game::InitSDL() {
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

	//Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	
	// Sets the color uses for rectangles, lines etc AND for clearing the screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Init SDL_Image with PNG support
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

bool Game::Load() {
	stateManager.Init("gamestate");
	statistics = std::make_unique<Statistics>(Statistics());
	return true;
}

void Game::GameLoop() {
	//const float dt = 0.16666; // 6000 updates/s
	const float dt = 16.666; // 60 updates/s
	float currentTime = SDL_GetTicks();
	float accumulator = 0.0;

	while (!quit) {		
		Uint32 newTime = SDL_GetTicks();
		float frameTime = newTime - currentTime;
		const Uint32 maxFrameTime = 1000; // 1 sec per frame is the slowest we allow
		if (frameTime > maxFrameTime)
			frameTime = maxFrameTime;
		
		currentTime = newTime;
		accumulator += frameTime;
		while (accumulator >= dt) {
			HandleInput();

			for (auto const &keys : keysDown) {
				stateManager.HandleEvent(keys.second);
			}
			stateManager.Update(dt);

		    accumulator -= dt;
			//t += dt;
			statistics->IncreaseUpdates();
		}
		
		stateManager.Draw();

		statistics->IncreaseFrames();
		statistics->Log();
	}
}

void Game::HandleInput() {
	//Event handler
	SDL_Event e;
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
			case SDLK_d:
				keysDown[e.key.keysym.sym] = MOVE_RIGHT;
				break;
			case SDLK_a:
				keysDown[e.key.keysym.sym] = MOVE_LEFT;
				break;
			case SDLK_w:
				keysDown[e.key.keysym.sym] = ACCELERATE;
				break;
			case SDLK_s:
				keysDown[e.key.keysym.sym] = DECELERATE;
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
		else if (e.type == SDL_KEYUP) {
			keysDown.erase(e.key.keysym.sym);
		}
	}
}
