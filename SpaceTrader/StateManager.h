#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include "State.h"
#include "Events.h"
#include "World.h"

class StateManager
{
public:
	StateManager(World world);
	~StateManager();
	void Init(std::string startingState);
	
	void Draw();
	void Update(float deltaTime);
	void HandleEvent(Event event);

	void PushState(std::string stateId);
	void PopState();
	void ClearStates();
	World GetWorld() const;
	SDL_Renderer* GetRenderer() const;
	void SetRenderer(SDL_Renderer* sdlRenderer);
	bool IsEmpty() const;
		
private:
	World world;
	SDL_Renderer* renderer;
	std::unordered_map<std::string, State*> availableStates;
	std::vector<State*> states;
	std::vector<std::function<void()>> pendingChanges;

	void ApplyPendingChanges();
};