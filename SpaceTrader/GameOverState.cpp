#include "GameOverState.h"
#include <iostream>

GameOverState::GameOverState(StateManager& stateManager)
 : State(stateManager) {
}

GameOverState::~GameOverState() {
	std::cout << "Game OVER State removed." << std::endl;
}

void GameOverState::Init() {
	std::cout << "Game OVER State Init." << std::endl;
}
	
bool GameOverState::Update(float deltaTime) {
	PopState();
	PushState("gamestate");
	return true;
}

void GameOverState::Draw() {
	std::cout << "Game OVER draw." << std::endl;
}

bool GameOverState::HandleEvent(Event event) {
	return true;
}