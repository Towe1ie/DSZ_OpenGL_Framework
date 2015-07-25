#include "Input.h"
#include "GameTime.h"
#include "Globals.h"
#include "Game.h"

Mouse Input::mouse = Mouse();
MouseState Input::currentMouseState = MouseState();
MouseState Input::previousMouseState = MouseState();

LRESULT CALLBACK Input::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//case WM_ACTIVATE:
	//{
	//	if (!HIWORD(wParam))
	//	{
	//		game->active = TRUE;
	//	}
	//	else
	//	{
	//		game->active = FALSE;
	//	}

	//	return 0;
	//}

	case WM_SYSCOMMAND:
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}

	case WM_CLOSE:
	{
		DestroyWindow(game->hwnd);
		game->Finish();
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}


	case WM_LBUTTONDOWN:
	{
		Input::mouse.updateButtonState(MouseButton::LEFT, ButtonState::DOWN);
		break;
	}
	case WM_LBUTTONUP:
	{
		Input::mouse.updateButtonState(MouseButton::LEFT, ButtonState::UP);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		Input::mouse.updateButtonState(MouseButton::RIGHT, ButtonState::DOWN);
		break;
	}
	case WM_RBUTTONUP:
	{
		Input::mouse.updateButtonState(MouseButton::RIGHT, ButtonState::UP);
		break;
	}
	case WM_MBUTTONUP:
	{
		Input::mouse.updateButtonState(MouseButton::MIDDLE, ButtonState::UP);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		Input::mouse.updateButtonState(MouseButton::MIDDLE, ButtonState::DOWN);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		Input::mouse.updateScrolState(GET_WHEEL_DELTA_WPARAM(wParam));
		return 0;
	}

	/*case WM_KEYDOWN:
	{
	keys[wParam] = TRUE;
	return 0;
	}

	case WM_KEYUP:
	{
	keys[wParam] = FALSE;
	return 0;
	}*/
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

MouseState Input::getMouseState()
{
	return currentMouseState;
}

void Input::Update(GameTime gameTime)
{
	mouse.updateScrolState(0);

	MSG msg;

	while (PeekMessage(&msg, game->hwnd, 0, 0, PM_REMOVE))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}


	mouse.Update(gameTime);

	previousMouseState = currentMouseState;
	currentMouseState = mouse.getMouseState();
}

ButtonState Input::GetButtonState(MouseButton button)
{
	return currentMouseState.buttonsStates[button];
}

bool Input::IsMouseButtonReleased(MouseButton button)
{
	return (currentMouseState.buttonsStates[button] == UP && previousMouseState.buttonsStates[button] == DOWN);
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
	return (currentMouseState.buttonsStates[button] == UP && previousMouseState.buttonsStates[button] == DOWN);
}

glm::vec2 Input::GetMouseCoordinates()
{
	glm::vec2 v = glm::vec2(currentMouseState.x, currentMouseState.y);

	return v;
}

glm::vec2 Input::GetNormalizedMouseCoordinates()
{
	return GetMouseCoordinates() / glm::vec2(game->Width(), game->Height());
}

glm::vec2 Input::GetPreviouseMouseCoordinates()
{
	glm::vec2 v = glm::vec2(previousMouseState.x, previousMouseState.y);

	return v;
}

float Input::GetScrollValue()
{
	return currentMouseState.scrolValue;
}