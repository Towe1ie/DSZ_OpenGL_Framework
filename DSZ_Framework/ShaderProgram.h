#ifndef _SHADERPROGRAM_H_
#define _SHADERPROGRAM_H_

#include <Windows.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "DSZMathHeader.h"



class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	char InstallShader(char *vsFileName, char *fsFileName); // 0 unsuccessful, 1 successful
	char UninstallShader();

	char Activate();
	char Deactivate();

	GLuint GetProgObject() const;

	bool SetAttribute(char* attributeName, const glm::mat4x4& matrix);
	bool SetAttribute(char* attributeName, const glm::vec4& vector);
	bool SetAttribute(char* attributeName, const glm::vec3& vector);
	bool SetAttribute(char* attributeName, const bool value);

private:
	GLuint vs, fs, prog;
};

#endif