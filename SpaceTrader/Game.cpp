#include "Game.h"

Game::Game() 
	: keysDown() {
	worldBounds = std::make_shared<SDL_Point>(SDL_Point{ SCREEN_WIDTH, SCREEN_HEIGHT });
	printf("Starting game \n");
}

Game::~Game() {
}

void Game::Run() {
	if (Init())
		GameLoop3();
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
	
	backgrounds[0] = LoadTexture("2.png");
	backgrounds[1] = LoadTexture("3.png");
	currentBackground = 0;
	shipTexture = LoadTexture("cruiser.png");
	ship = make_unique<Ship>(Ship(shipTexture.get(), new SDL_Point{ 100, 100 }, worldBounds));
	return true;
}

void Game::GameLoop() {
	while (!quit) {
		HandleInput();
		Update(0);
		Render();
	}
}

void Game::GameLoop2() {
	fps_lasttime = SDL_GetTicks();
	Uint32 next_game_tick = SDL_GetTicks();
	fps_frames = 0;
	int loops;
	int updates = 0;
	while (!quit) {
		loops = 0;
		while (SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP) {
			HandleInput();
			Update(0);
			
			next_game_tick += SKIP_TICKS;
			loops++;
			updates++;
		}

		Render();
		fps_frames++;

		if (fps_lasttime < SDL_GetTicks() - 1000.0) {
			fps_lasttime = SDL_GetTicks();
			fps_current = fps_frames;
			printf("updates/s: %i ,", updates);
			printf("fps: %i\n", fps_current);
			updates = 0;
			fps_frames = 0;
		}
	}
}

void Game::GameLoop3() {
	//float t = 0.0;
	const float dt = 0.16666; // 6000 updates/s
	float currentTime = SDL_GetTicks();
	float accumulator = 0.0;
	int updates = 0;

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
		    Update(dt); // simulate a "frame" of logic at a different FPS than we simulate a frame of rendering
		    accumulator -= dt;
			//t += dt;
			updates++;
		}
		
		Render();
		fps_frames++;
		
		if (fps_lasttime < SDL_GetTicks() - 1000.0) {
			fps_lasttime = SDL_GetTicks();
			fps_current = fps_frames;
			printf("updates/s: %i ,", updates);
			printf("fps: %i\n", fps_current);
			updates = 0;
			fps_frames = 0;
		}
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
				currentBackground = 1;
				break;
			case SDLK_2:
				currentBackground = 0;
				break;
			case SDLK_d:
				keysDown[e.key.keysym.sym] = MOVE_RIGHT;
				break;
			case SDLK_a:
				keysDown[e.key.keysym.sym] = MOVE_LEFT;
				break;
			case SDLK_w:
				ship->HandleEvent(ACCELERATE);
				break;
			case SDLK_s:
				ship->HandleEvent(DECELERATE);
				break;
			case SDLK_q:
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

void Game::Update(float dt) {
	for (auto const &it1 : keysDown) {
		ship->HandleEvent(it1.second);
	}
	ship->Update(dt);
}

void Game::Render() {
	//Clear screen
	SDL_RenderClear(renderer);
	//Render texture to screen
	SDL_RenderCopy(renderer, backgrounds[currentBackground].get(), NULL, NULL);

	ship->Render(renderer);

	//Update screen
	SDL_RenderPresent(renderer);
}

TexturePtr Game::LoadTexture(std::string path) {
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