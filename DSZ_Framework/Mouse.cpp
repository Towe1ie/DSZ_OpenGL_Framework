#include "Mouse.h"
#include "Game.h"
#include "Globals.h"

MouseState::MouseState()
{
	buttonsStates[0] = UP;
	buttonsStates[1] = UP;
	buttonsStates[2] = UP;
	x = 0;
	y = 0;
	scrolValue = 0;
}

void Mouse::updateCoordinates()
{
	POINT p;

	if (GetCursorPos(&p))
	{
		if (ScreenToClient(game->hwnd, &p))
		{
			mouseState.x = (float)p.x;
			mouseState.y = (float)p.y;
		}
	}
}

void Mouse::updateButtonState(MouseButton button, ButtonState state)
{
	mouseState.buttonsStates[button] = state;
}

void Mouse::updateScrolState(float value)
{
	mouseState.scrolValue = value;
}

MouseState Mouse::getMouseState() const
{
	return mouseState;
}

void Mouse::Update(GameTime gameTime)
{
	updateCoordinates();
}