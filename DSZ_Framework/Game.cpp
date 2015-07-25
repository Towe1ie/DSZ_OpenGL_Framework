#include <glew.h>
#include <wglew.h>
#include "Game.h"
#include "GameTime.h"
#include "Globals.h"
#include "Input.h"
#include <iostream>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "WinUtils.h"
#include "HelperFunctions.h"

#define WIDTH 1920
#define HEIGHT 1080

Game::Game()
	:done(false), bound_fps(60.0), total_frames(0), fullscreen(true)
{
	gameTime = new GameTime();
}

Game::~Game()
{
	UnloadContent();
	delete gameTime;
	destroyContext(hRC);
	destroyWindow(hwnd);
}

void Game::Run()
{
	game = this;
	::initExtensions();

	this->hwnd = ::createWindow("Title", WIDTH, HEIGHT, 32, fullscreen);
	this->hRC = ::createContext(hwnd, 4, 5);
	this->hDC = GetDC(hwnd);
	this->width = WIDTH;
	this->height = HEIGHT;

	initGL();

	Initialize();
	LoadContent();

	PrintGlVersion(stdout);
	PrintGlslVersion(stdout);

	gameLoop();
}

void Game::Finish()
{
	done = true;
}

void Game::gameLoop()
{
	double one_sec = 0.0;

	while (!done)
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		this->gameTime->currentTime = currentTime;

		double elapsed = gameTime->dt();

		if (elapsed >= 1 / bound_fps)
		{
			Input::Update(*gameTime);
			Update(*gameTime);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Draw();
			SwapBuffers(hDC);

			MSG msg;

			while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			total_frames++;
			one_sec += elapsed;

			QueryPerformanceCounter(&currentTime);
			gameTime->lastUpdateTime = currentTime;


			if (one_sec >= 1.0)
			{
				printf("FPS = %d\n", total_frames);
				total_frames = 0;
				one_sec = 0.0;
			}
		}
		else
		{
			//Sleep((DWORD)(2000.0 / 3 * (1 / bound_fps - elapsed))); // uspavaj za 2/3 preostalog vremena
		}


	}
}

bool Game::initGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(100.0f / 255, 149.f / 255, 237.f / 255, 1.f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return TRUE;
}

void Game::LoadContent() {}

void Game::UnloadContent() {}

void Game::Initialize() {}

int Game::Width() const
{
	return width;
}

int Game::Height() const
{
	return height;
}