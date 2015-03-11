#pragma once
#include <SDL.h>
#include <stdio.h>

class Statistics
{
public:
	Statistics();
	~Statistics();
	void IncreaseFrames();
	void IncreaseUpdates();
	void Log();
private:
	Uint32 updates = 0;
	Uint32 frames = 0;
	Uint32 lasttime; 
};

