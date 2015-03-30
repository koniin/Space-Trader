#include "Game.h"
#include "Ship.h"
#include "Station.h"

Game::Game() 
	: keysDown() {
	worldBounds = SDL_Point{ LEVEL_WIDTH, LEVEL_HEIGHT };
	printf("Starting game \n");
}

Game::~Game() {
}

void Game::Run() {
	if (InitSDL()) {
		Load();
		GameLoop3();
	} 

	TTF_CloseFont(font);
	font = NULL;
	//SDL_DestroyRenderer(renderer);  // slow ?
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
	font = LoadFont("ABSTRACT.ttf");
	statistics = make_unique<Statistics>(Statistics());
	backgroundLayers[0] = LoadTexture("2.png");
	backgroundLayers[1] = LoadTexture("starfield.png");
	shipTexture = LoadTexture("cruiser.png");
	stationTexture = LoadTexture("station.png");
	ship = unique_ptr<Ship>{ make_unique<Ship>(Ship(shipTexture.get(), new SDL_Point{ 100, 100 }, &worldBounds))};
	station = unique_ptr<GameObject>{ make_unique<Station>(Station(stationTexture.get(), new SDL_Point{ 400, 400 }, &worldBounds))};

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

	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	const float dt = 0.16666; // 6000 updates/s
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
		    Update(dt); // simulate a "frame" of logic at a different FPS than we simulate a frame of rendering
		    accumulator -= dt;
			//t += dt;
			statistics->IncreaseUpdates();
		}
		
		Render();
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
				ship->HandleEvent(ACCELERATE);
				break;
			case SDLK_s:
				ship->HandleEvent(DECELERATE);
				break;
			case SDLK_e:
				ship->HandleEvent(STRAFE_RIGHT);
				break;
			case SDLK_q:
				ship->HandleEvent(STRAFE_LEFT);
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

void Game::Update(float dt) {
	for (auto const &it1 : keysDown) {
		ship->HandleEvent(it1.second);
	}
	ship->Update(dt);
	station->Update(dt);

	CheckCollisions();
	
	UpdateCamera();
}

void Game::CheckCollisions() {
	if (!station->IsDestroyed() && SDL_HasIntersection(ship->GetPositionRectangle(), station->GetPositionRectangle())) {
		station->Collide(ship.get());
		ship->Collide(station.get());
	}
}

void Game::UpdateCamera() {
	const SDL_Rect* shipPos = ship->GetPositionRectangle();
	camera.x = (shipPos->x + shipPos->w / 2) - SCREEN_WIDTH / 2;
	camera.y = (shipPos->y + shipPos->h / 2) - SCREEN_HEIGHT / 2;
	camera.x = std::max(0, std::min(camera.x, LEVEL_WIDTH - camera.w));
	camera.y = std::max(0, std::min(camera.y, LEVEL_HEIGHT - camera.h));
}

void Game::Render() {
	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);

	//Render background
	SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderCopy(renderer, backgroundLayers[0].get(), &camera, &renderQuad);
	
	ship->Render(renderer, camera.x, camera.y);
	station->Render(renderer, camera.x, camera.y);

	RenderText("Cargo: " + std::to_string(ship->GetCargo()));

	//Update screen
	SDL_RenderPresent(renderer);
}

void Game::RenderText(string text) {
	//Render text surface
	SDL_Color textColor = { 93, 149, 212 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Create texture from surface pixels
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		} else {
			SDL_Rect renderQuad = { 10, SCREEN_HEIGHT - textSurface->h * 1.5f, textSurface->w, textSurface->h };
			SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE);
			SDL_DestroyTexture(texture);
    			texture = NULL;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
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

TTF_Font* Game::LoadFont(string path) {
	//Open the font
	font = TTF_OpenFont(path.c_str(), 8);
	if (font == NULL) {
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return NULL;
	}
	return font;
}
