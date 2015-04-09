#include "GameState.h"
#include "World.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

GameState::GameState(StateManager& stateManager)
 : State(stateManager) {
}

GameState::~GameState() {
	std::cout << "GAMESTATE removed." << std::endl;
	TTF_CloseFont(font);
	font = NULL;
}

void GameState::Init() {
	elapsedTime = 0;
	std::cout << "GAMESTATE Init." << std::endl;
	auto world = GetWorld();
	camera = { 0, 0, world.SCREEN_WIDTH, world.SCREEN_HEIGHT };
	
	font = LoadFont("04B_03.ttf", 8);
	backgroundLayers[0] = LoadTexture("2.png");
	backgroundLayers[1] = LoadTexture("starfield.png");

	worldBounds = SDL_Point{ world.LEVEL_WIDTH, world.LEVEL_HEIGHT };

	shipTexture = LoadTexture("Ship3.png");
	ship = std::unique_ptr<Ship>{ std::make_unique<Ship>(Ship(shipTexture.get(), new SDL_Point{ 100, 100 }, &worldBounds))};

	stationTexture = LoadTexture("Cargo.png");
	stations.push_back(std::unique_ptr<GameObject>{ std::make_unique<Station>(Station(stationTexture.get(), new SDL_Point{ 100, 500 }, &worldBounds))});
	stations.push_back(std::unique_ptr<GameObject>{ std::make_unique<Station>(Station(stationTexture.get(), new SDL_Point{ 1700, 500 }, &worldBounds))});
	stations.push_back(std::unique_ptr<GameObject>{ std::make_unique<Station>(Station(stationTexture.get(), new SDL_Point{ 1700, 300 }, &worldBounds))});
	stations.push_back(std::unique_ptr<GameObject>{ std::make_unique<Station>(Station(stationTexture.get(), new SDL_Point{ 1700, 1700 }, &worldBounds))});

}

bool GameState::Update(float deltaTime) {
	elapsedTime += deltaTime;
	msCounter += deltaTime;
	timeLeft = 300000 - elapsedTime;
	if (msCounter > 999)
		msCounter = 0;

	ship->Update(deltaTime);

	for (const auto &station : stations) {
		station->Update(deltaTime);
	}

	CheckCollisions();

	if (ship->GetResources() >= 1000) {
		PopState();
		PushState("gameoverstate");
	}
	else if (timeLeft <= 0) {
		PopState();
		PushState("gameoverstate");
	}

	UpdateCamera();
	
	return true;
}

void GameState::Draw() {
	SDL_Renderer* renderer = GetRenderer();

	//Clear screen
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);

	//Render background
	auto world = GetWorld();
	SDL_Rect renderQuad = { 0, 0, world.SCREEN_WIDTH, world.SCREEN_HEIGHT };
	SDL_RenderCopy(renderer, backgroundLayers[0].get(), &camera, &renderQuad);

	ship->Render(renderer, camera.x, camera.y);
	for (const auto &station : stations) {
		station->Render(renderer, camera.x, camera.y);
	}

	RenderText("Cargo: " + std::to_string(ship->GetCargo()) + "  Resources: " + std::to_string(ship->GetResources()));

	RenderText(GetTimerText(), { (world.SCREEN_WIDTH / 2) - 30, 10 }, { 255, 255, 255 });

	//Update screen
	SDL_RenderPresent(renderer);
}

bool GameState::HandleEvent(Event event) {
	ship->HandleEvent(event);
	return true;
}

void GameState::UpdateCamera() {
	auto world = GetWorld();
	const SDL_Rect* shipPos = ship->GetPositionRectangle();
	camera.x = (shipPos->x + shipPos->w / 2) - world.SCREEN_WIDTH / 2;
	camera.y = (shipPos->y + shipPos->h / 2) - world.SCREEN_HEIGHT / 2;
	camera.x = std::max(0, std::min(camera.x, world.LEVEL_WIDTH - camera.w));
	camera.y = std::max(0, std::min(camera.y, world.LEVEL_HEIGHT - camera.h));
}

void GameState::CheckCollisions() {
	for (const auto &station : stations) {
		if (!station->IsDestroyed() && SDL_HasIntersection(ship->GetPositionRectangle(), station->GetPositionRectangle())) {
			station->Collide(ship.get());
			ship->Collide(station.get());
		}
	}
}

void GameState::RenderText(std::string text) {
	//Render text surface
	SDL_Color textColor = { 93, 149, 212 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Create texture from surface pixels
		SDL_Texture* texture = SDL_CreateTextureFromSurface(GetRenderer(), textSurface);
		if (texture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			auto world = GetWorld();
			SDL_Rect renderQuad = { 10, world.SCREEN_HEIGHT - textSurface->h * 1.5f, textSurface->w, textSurface->h };
			SDL_RenderCopyEx(GetRenderer(), texture, NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE);
			SDL_DestroyTexture(texture);
			texture = NULL;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
}

void GameState::RenderText(std::string text, SDL_Point pos, SDL_Color textColor) {
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Create texture from surface pixels
		SDL_Texture* texture = SDL_CreateTextureFromSurface(GetRenderer(), textSurface);
		if (texture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			SDL_Rect renderQuad = { pos.x, pos.y, textSurface->w, textSurface->h };
			SDL_RenderCopyEx(GetRenderer(), texture, NULL, &renderQuad, NULL, NULL, SDL_FLIP_NONE);
			SDL_DestroyTexture(texture);
			texture = NULL;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
}

TexturePtr GameState::LoadTexture(std::string path) {
	//The final texture
	TexturePtr newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		//Create texture from surface pixels
		newTexture = TexturePtr(SDL_CreateTextureFromSurface(GetRenderer(), loadedSurface));
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

TTF_Font* GameState::LoadFont(std::string path, int fontSize) {
	//Open the font
	font = TTF_OpenFont(path.c_str(), fontSize);
	if (font == NULL) {
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return NULL;
	}
	return font;
}

std::string GameState::GetTimerText() {
	std::stringstream ss;
	float seconds = (int)(timeLeft / 1000) % 60;
	float minutes = (int)(timeLeft / (1000 * 60)) % 60;
	float milliseconds = 100 - (msCounter / 10);
	ss << std::setfill('0') << std::setw(2) << minutes << " : " << std::setfill('0') << std::setw(2) << seconds << " : " << std::setfill('0') << std::setw(2) << std::round(milliseconds);
	return ss.str();
}