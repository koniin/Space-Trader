#pragma once
#include "State.h"

class GameOverState : public State {
	public:
		GameOverState(StateManager& stateManager);
		~GameOverState();
		
		void Init();
		bool Update(float deltaTime);
		void Draw();
		bool HandleEvent(Event event);
};