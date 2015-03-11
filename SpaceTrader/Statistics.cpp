#include "Statistics.h"


Statistics::Statistics() {
	lasttime = SDL_GetTicks();
}


Statistics::~Statistics()
{
}

void Statistics::IncreaseFrames() {
	frames++;
}

void Statistics::IncreaseUpdates() {
	updates++;
}

void Statistics::Log() {
	if (lasttime < SDL_GetTicks() - 1000.0) {
		lasttime = SDL_GetTicks();
		printf("updates/s: %i ,", updates);
		printf("fps: %i\n", frames);
		updates = 0;
		frames = 0;
	}
}