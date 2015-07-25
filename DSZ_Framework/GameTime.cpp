#include "GameTime.h"

GameTime::GameTime()
{
	QueryPerformanceCounter(&currentTime);
	QueryPerformanceCounter(&lastUpdateTime);
	QueryPerformanceFrequency(&clock);
}

double GameTime::dt()
{
	LARGE_INTEGER elapsed;
	elapsed.QuadPart = currentTime.QuadPart - lastUpdateTime.QuadPart;

	double dt = ((double)elapsed.QuadPart) / clock.QuadPart;

	return dt;
}