#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 
#include <memory>
#include <vector>
#include "State.h"
#include "Ship.h"
#include "Station.h"

struct _TextureDeleter { void operator()(SDL_Texture* tex) const { SDL_DestroyTexture(tex); printf("texture deleted. \n"); } };
using TexturePtr = std::unique_ptr<SDL_Texture, _TextureDeleter>;

class GameState : public State {
private:
	SDL_Rect camera;
	std::unique_ptr<Ship> ship;
	std::vector<std::unique_ptr<GameObject>> stations;
	TexturePtr backgroundLayers[2];
	TexturePtr shipTexture;
	TexturePtr stationTexture;
	TTF_Font* font = NULL;
	SDL_Point worldBounds;

	void CheckCollisions();
	void UpdateCamera();
	void RenderText(std::string text);
	TexturePtr LoadTexture(std::string path);
	TTF_Font* LoadFont(std::string path, int fontSize);
public:
	GameState(StateManager& stateManager);
	~GameState();
	
	void Init();
	bool Update(float deltaTime);
	void Draw();
	bool HandleEvent(Event event);
};