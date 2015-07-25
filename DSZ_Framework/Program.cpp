#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "Game.h"
#include "Game1.h"
#include "Globals.h"

int WINAPI WinMain(HINSTANCE   hInstance,              // Instance
	HINSTANCE   hPrevInstance,              // Previous Instance
	LPSTR       lpCmdLine,              // Command Line Parameters
	int     nCmdShow)
{
	AllocConsole();
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;
	printf("Console Allocated\n");


	Game *game = new Game1();
	hInstance = hInstance;
	game->Run();

	FreeConsole();
	return 0;
}