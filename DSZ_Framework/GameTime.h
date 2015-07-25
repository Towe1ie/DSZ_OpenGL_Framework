#pragma once

#include <Windows.h>

class Game;

class GameTime
{
private:
	LARGE_INTEGER currentTime;
	LARGE_INTEGER lastUpdateTime;

	LARGE_INTEGER clock;

	friend class Game;

public:
	GameTime();

	double dt();
};