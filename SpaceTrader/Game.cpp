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
	
	background = LoadTexture("2.png");
	
	return true;
}

void Game::GameLoop() {
	while (!quit) {
		HandleInput();
		Render();
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
			case SDLK_1:
				background = LoadTexture("3.png");
				break;
			case SDLK_2:
				background = LoadTexture("2.png");
				break;
			case SDLK_q:
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
	}
}

void Game::Render() {
	//Clear screen
	SDL_RenderClear(renderer);
	//Render texture to screen
	SDL_RenderCopy(renderer, background.get(), NULL, NULL);
	//Update screen
	SDL_RenderPresent(renderer);
}

TexturePtr Game::LoadTexture(std::string path)
{
	//The final texture
	TexturePtr newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//Create texture from surface pixels
		newTexture = TexturePtr(SDL_CreateTextureFromSurface(renderer, loadedSurface));
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}