#ifndef _GRID_H_
#define _GRID_H_

#include "ShaderProgram.h"

class Grid
{
private:
	int size;

public:
	Grid(int size = 10);

	void draw();

	float array[6];

	GLuint vao, vbo, cvbo;
	ShaderProgram shader;
};

#endif