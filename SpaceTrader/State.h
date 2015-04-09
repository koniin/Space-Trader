#pragma once
#include <string>
#include <SDL.h>
#include "Events.h"
#include "World.h"
class StateManager;
struct World;

class State {
	public:
		State(StateManager& stateManager);
		~State();
		
		virtual void Init() = 0;
		virtual bool Update(float deltaTime) = 0;
		virtual void Draw() = 0;
		virtual bool HandleEvent(Event event) = 0;
	protected:
		void PopState();
		void PushState(std::string stateId);
		void ClearStates();
		World GetWorld() const;
		SDL_Renderer* GetRenderer() const;
	private:
		StateManager* states;
};