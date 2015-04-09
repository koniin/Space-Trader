#include <SDL.h>
#include "State.h"
#include "StateManager.h"

State::State(StateManager& stateManager) 
	: states(&stateManager) {
}

State::~State() {
}

void State::PopState() {
	states->PopState();
}

void State::PushState(std::string stateId) {
	states->PushState(stateId);
}

void State::ClearStates() {
	states->ClearStates();
}

World State::GetWorld() const {
	return states->GetWorld();
}

SDL_Renderer* State::GetRenderer() const {
	return states->GetRenderer();
}