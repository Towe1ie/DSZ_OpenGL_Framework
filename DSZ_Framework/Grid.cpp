#include <glew.h>
#include "Grid.h"
#include <math.h>
#include "DSZMathHeader.h"
#include "DSZCore.h"
#include <gl\GL.h>
#include <gl\GLU.h>


using namespace glm;

Grid::Grid(int size)
: size(size)
{
	array[0] = 0;
	array[1] = 0;
	array[2] = size / 2.f;

	array[3] = 0;
	array[4] = 0;
	array[5] = -size / 2.f;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), array, GL_STATIC_DRAW);

	array[0] = 0.5f;
	array[1] = 0.5f;
	array[2] = 0.5f;

	array[3] = 0.5f;
	array[4] = 0.5f;
	array[5] = 0.5f;

	glGenBuffers(1, &cvbo);
	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), array, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Grid::draw()
{
	glBindVertexArray(vao);
	
	SimpleShader.Activate();

	mat4x4 m;
	mat4x4 m1;

	SimpleShader.SetAttribute("UseLighting", false);
	SimpleShader.SetAttribute("UseStaticColor", true);
	SimpleShader.SetAttribute("StaticColor", vec4(0.0, 1.0, 0.0, 1.0));

	m1 *= glm::translate(vec3(-size / 2.f, 0.0f, 0.0f));
	for (int i = 0; i <= size; i++)
	{
		SimpleShader.SetAttribute("World", m1);
		glDrawArrays(GL_LINES, 0, 2);
		m1 *= glm::translate(vec3(1.0f, 0.0f, 0.0f));
	}

	m1 = m;
	m1 *= glm::rotate((float)M_PI_2, vec3(0, 1, 0));
	m1 *= glm::translate(vec3(-size / 2.f, 0.0f, 0.0f));

	for (int i = 0; i <= size; i++)
	{
		SimpleShader.SetAttribute("World", m1);
		glDrawArrays(GL_LINES, 0, 2);
		m1 *= glm::translate(vec3(1.0f, 0.0f, 0.0f));
	}

	//SimpleShader.SetAttribute("UseLighting", true);
	SimpleShader.SetAttribute("World", m);

}