#pragma once

#include "Game.h"

class Game1 : public Game
{
public:
	virtual void Update(GameTime gameTime);
	virtual void Draw();
	virtual void Initialize();
	virtual void LoadContent();
	virtual void UnloadContent();

	virtual Game1::~Game1();
};