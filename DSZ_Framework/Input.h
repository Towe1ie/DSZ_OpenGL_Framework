#pragma once

#include <Windows.h>
#include "Mouse.h"
#include <glm\vec2.hpp>
class GameTime;

class Input
{
private:
	static Mouse mouse;

	static MouseState currentMouseState, previousMouseState;

public:

	static MouseState getMouseState();

	static ButtonState GetButtonState(MouseButton button);

	static bool IsMouseButtonReleased(MouseButton button);

	static bool IsMouseButtonPressed(MouseButton button);

	static glm::vec2 GetMouseCoordinates();

	static glm::vec2 GetNormalizedMouseCoordinates();

	static glm::vec2 GetPreviouseMouseCoordinates();

	static float GetScrollValue();

	static void Update(GameTime gameTime);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};