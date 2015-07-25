#ifndef _HELPERFUNCTIONS_H_
#define _HELPERFUNCTIONS_H_

#include <stdio.h>
#include <Windows.h>
#include <glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "BoundingBox.h"
#include <glm\glm.hpp>
//#include "glext.h"

// **** OpenGL Info ****

void GetGlVersion(int *major, int *minor);
void GetGlslVersion(int *major, int *minor);

void PrintGlVersion(FILE* out);
void PrintGlslVersion(FILE *out);

bool CheckExtension(const char *ext);
void PrintAllExtensions(FILE *out);

int MaxVertexAtrib();

// **** Log Functions ****

/* Shader type : 
	0 : Vertex Shader
	1 : Fragment Shader
*/
void PrintShaderInfoLog(GLuint shader, char shaderType, char* fileName);

void PrintProgramInfoLog(GLuint program);


// **** Other Functions ****

char* CopyFileToString(FILE *input, int* length);

// **** Debug Functions ****

void DrawBoundingBox(BoundingBox &box);
void DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 color);

#endif