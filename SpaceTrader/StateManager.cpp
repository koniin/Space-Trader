#include <iostream>
#include "assert.h"

#include "StateManager.h"
#include "State.h"
#include "GameState.h"
#include "GameOverState.h"

StateManager::StateManager(World worldParams) 
	: world(worldParams) {
}

StateManager::~StateManager(void) {
}

void StateManager::Init(std::string startingState) {
	std::cout << "adding states" << std::endl;
	availableStates.emplace("gamestate", new GameState(*this));
	availableStates.emplace("gameoverstate", new GameOverState(*this));
	
	PushState(startingState);
}

void StateManager::Update(float deltaTime) {
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = states.rbegin(); itr != states.rend(); ++itr) {
		if (!(*itr)->Update(deltaTime))
			break;
	}
	
	ApplyPendingChanges();
}

void StateManager::Draw() {
	// draw from bottom to top (so the last state gets drawn last, e.g. a pause layer)
	for (auto itr = states.begin(); itr != states.end(); ++itr) {
		(*itr)->Draw();
	}
}

void StateManager::HandleEvent(Event event) {
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = states.rbegin(); itr != states.rend(); ++itr)
	{
		if (!(*itr)->HandleEvent(event))
			break;
	}
	
	ApplyPendingChanges();
}

void StateManager::PushState(std::string stateId) {
	pendingChanges.push_back([=] () {
		assert(availableStates.find(stateId) != availableStates.end() && "couldnt find state");
		availableStates[stateId]->Init();
		states.push_back(availableStates[stateId]);
	});
}

void StateManager::PopState() {
	pendingChanges.push_back([this] () { 
		assert(!states.empty() && "pop on empty");
		states.pop_back(); 
	});
}

void StateManager::ClearStates() {
	pendingChanges.push_back([this] () { states.clear(); });
}

bool StateManager::IsEmpty() const {
	return states.empty();
}

void StateManager::ApplyPendingChanges() {
	for (auto & change : pendingChanges) {
		change();
	}
	pendingChanges.clear();
}

World StateManager::GetWorld() const {
	return world;
}

SDL_Renderer* StateManager::GetRenderer() const {
	return renderer;
}

void StateManager::SetRenderer(SDL_Renderer* sdlRenderer) {
	renderer = sdlRenderer;
}