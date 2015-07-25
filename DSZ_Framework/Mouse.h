

#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "GameTime.h"

typedef enum ButtonState { DOWN, UP };
typedef enum MouseButton { LEFT, MIDDLE, RIGHT };

struct MouseState
{
	float x, y;
	ButtonState buttonsStates[3];
	float scrolValue;

	MouseState();
};

class Mouse
{
private:
	MouseState mouseState;

	void updateCoordinates();
	
	void updateButtonState(MouseButton button, ButtonState state);

	void updateScrolState(float value);

public:

	MouseState getMouseState() const;

	void Update(GameTime gameTime);

	friend class Input;
};

#endif

