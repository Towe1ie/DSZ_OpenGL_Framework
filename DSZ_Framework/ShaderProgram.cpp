#include "ShaderProgram.h"
#include <cstring>
#include <stdio.h>
#include "HelperFunctions.h"
#include "DSZCore.h"

using namespace glm;


ShaderProgram::ShaderProgram()
: vs(0), fs(0), prog(0)
{
}

ShaderProgram::~ShaderProgram()
{
	UninstallShader();
}

char ShaderProgram::InstallShader(char *vsFileName, char *fsFileName)
{
	UninstallShader();

	FILE *vsFile, *fsFile;
	if (!(vsFile = fopen(vsFileName, "r")))
		return 0;
	if (!(fsFile = fopen(fsFileName, "r")))
	{
		fclose(vsFile);
		return 0;
	}

	const GLchar *vsCode, *fsCode;
	int vsLength, fsLength;
	vsCode = CopyFileToString(vsFile, &(vsLength));
	fsCode = CopyFileToString(fsFile, &(fsLength));

	fclose(vsFile);
	fclose(fsFile);

	GLint vsCompiled, fsCompiled;
	GLint linked;

	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsCode, NULL);
	glShaderSource(fs, 1, &fsCode, NULL);

	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &vsCompiled);

	PrintShaderInfoLog(vs, 0, vsFileName);

	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &fsCompiled);
	PrintShaderInfoLog(fs, 1, fsFileName);

	delete[] vsCode;
	delete[] fsCode;

	if (!vsCompiled || !fsCompiled)
		return 0;

	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	PrintProgramInfoLog(prog);

	if (!linked)
		return 0;

	return 1;
}

char ShaderProgram::UninstallShader()
{
	if (!vs && !fs && !prog)
		return 0;


	glDetachShader(prog, vs);
	glDetachShader(prog, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(0);
	glDeleteProgram(prog);



	fs = vs = prog = 0;

	return 1;
}

char ShaderProgram::Activate()
{
	if (!prog)
		return 0;

	glUseProgram(prog);

	return 1;
}

char ShaderProgram::Deactivate()
{
	if (!prog)
		return 0;

	glUseProgram(0);

	return 1;
}

GLuint ShaderProgram::GetProgObject() const
{
	return prog;
}

bool ShaderProgram::SetAttribute(char* attributeName, const mat4x4& matrix)
{
	int location = glGetUniformLocation(prog, attributeName);
	if (location == -1)
		return false;

	int currentProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProg);
	if (currentProg != this->prog)
		glUseProgram(this->prog);

	glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&matrix);

	if (currentProg != this->prog)
		glUseProgram(currentProg);

	return true;
}

bool ShaderProgram::SetAttribute(char* attributeName, const vec4& vector)
{
	int location = glGetUniformLocation(prog, attributeName);
	if (location == -1)
		return false;

	int currentProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProg);
	if (currentProg != this->prog)
		glUseProgram(this->prog);

	glUniform4f(location, vector[0], vector[1], vector[2], vector[3]);

	if (currentProg != this->prog)
		glUseProgram(currentProg);

	return true;
}

bool ShaderProgram::SetAttribute(char* attributeName, const vec3& vector)
{
	int location = glGetUniformLocation(prog, attributeName);
	if (location == -1)
		return false;

	int currentProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProg);
	if (currentProg != this->prog)
		glUseProgram(this->prog);

	glUniform3f(location, vector[0], vector[1], vector[2]);

	if (currentProg != this->prog)
		glUseProgram(currentProg);

	return true;
}

bool ShaderProgram::SetAttribute(char* attributeName, const bool value)
{
	int location = glGetUniformLocation(prog, attributeName);
	if (location == -1)
		return false;

	int currentProg;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProg);
	if (currentProg != this->prog)
		glUseProgram(this->prog);

	glUniform1i(location, value);

	if (currentProg != this->prog)
		glUseProgram(currentProg);

	return true;
}