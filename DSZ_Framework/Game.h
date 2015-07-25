#pragma once

#include <Windows.h>

class GameTime;

class Game
{
public:
	Game();
	virtual ~Game();

	void Run();
	void Finish();

	bool initExtensions();
	bool initGL();

	virtual void Update(GameTime gameTime) = 0;
	virtual void Draw() = 0;
	virtual void Initialize();
	virtual void LoadContent();
	virtual void UnloadContent();

	int Width() const;
	int Height() const;

private:
	GameTime *gameTime;
	void gameLoop();
	bool done;

	double bound_fps;
	int total_frames;

	bool fullscreen;

	HWND hwnd;
	HGLRC hRC;
	HDC hDC;
	int width, height;

	friend int WINAPI WinMain(HINSTANCE hIns,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow);
	friend class Input;
	friend class Mouse;
};