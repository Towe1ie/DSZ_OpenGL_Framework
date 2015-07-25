#include "Game1.h"
#include "GameTime.h"
#include "Grid.h"
#include "Globals.h"
#include "Camera.h"
#include "DSZMathHeader.h"

using namespace glm;

static Grid *grid;
static Camera *camera;

void Game1::Update(GameTime gameTime)
{
	camera->Update(gameTime);
}

void Game1::Draw()
{
	SimpleShader.SetAttribute("View", camera->GetViewMatrix());
	SimpleShader.SetAttribute("Projection", camera->GetProjection());

	grid->draw();
}

void Game1::Initialize()
{
	grid = new Grid(20);
	camera = new Camera(45.0f, ((float)Width()/Height()), 0.1f, 100000.0f, 50, vec3(0, 0, 0), -(float)(M_PI / 4.0f), -(float)(M_PI / 4.0f));
}

void Game1::LoadContent()
{
	SimpleShader.InstallShader("vs1.vert", "fs1.frag");
}

void Game1::UnloadContent()
{

}

Game1::~Game1()
{
	delete grid;
	delete camera;
}