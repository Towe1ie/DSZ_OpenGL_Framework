#include <glew.h>
#include "HelperFunctions.h"
#include <stdio.h>
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "glext.h"
#include "Globals.h"
#include "ShaderProgram.h"

//#define GL_GLEXT_PROTOTYPES
//#include "Extensions.h"




// **** OpenGL Info ****

void GetGlVersion(int *major, int *minor)
{
	const char *verstr = (const char*)glGetString(GL_VERSION);
	if ((verstr == NULL) || (sscanf_s(verstr, "%d.%d", major, minor) != 2))
	{
		*major = *minor = 0;
		fprintf(stderr, "Invalid GL_VERSION format!!!\n");
	}
}

void GetGlslVersion(int *major, int *minor)
{
	int gl_major, gl_minor;
	GetGlVersion(&gl_major, &gl_minor);
	*major = *minor = 0;
	if (gl_major == 1)
	{
		/* GL v1.x can only provide GLSL v1.00 as an extension */
		const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
		if ((extstr != NULL) &&
			(strstr(extstr, "GL_ARB_shading_language_100") != NULL))
		{
			*major = 1;
			*minor = 0;
		}
	}
	else if (gl_major >= 2)
	{
		/* GL v2.0 and greater must parse the version string */
		const char *verstr =
			(const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
		if ((verstr == NULL) ||
			(sscanf_s(verstr, "%d.%d", major, minor) != 2))
		{
			*major = *minor = 0;
			fprintf(stderr,
				"Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
		}
	}
}

void PrintGlVersion(FILE *out)
{
	const char *verstr = (const char*)glGetString(GL_VERSION);
	if (verstr != NULL)
		fprintf(out, "OpenGL version %s\n", verstr);
	else
		fprintf(out, "Invalid GL_VERSION format!!!\n");
}

void PrintGlslVersion(FILE *out)
{
	int gl_major, gl_minor;
	GetGlVersion(&gl_major, &gl_minor);

	if (gl_major == 1)
	{
		/* GL v1.x can only provide GLSL v1.00 as an extension */
		const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
		if (extstr != NULL && strstr(extstr, "GL_ARB_shading_language_100") != NULL)
			fprintf(out, "GLSL Version 1.0\n");
	}
	else if (gl_major >= 2)
	{
		const char *verstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
		if (verstr != NULL)
		{
			fprintf(out, "GLSL Version %s\n", verstr);
		}
		else
			fprintf(out, "Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
	}
}

bool CheckExtension(const char *ext)
{
	const GLubyte *extstr = glGetString(GL_EXTENSIONS);
	if (strstr((char*)extstr, ext) != NULL)
		return true;

	return false;
}

void PrintAllExtensions(FILE *out)
{
	const GLubyte *extstr = glGetString(GL_EXTENSIONS);
	char *extensions = new char[strlen((char*)extstr) + 1];
	const char *startaddr = extensions;
	strcpy(extensions, (char*)extstr);

	fprintf(out, "Extensions:\n");

	while (true)
	{
		char *substr = substr = strtok(extensions, " ");
		if (substr == NULL)
			break;
		extensions += strlen(substr) + 1;
		fprintf(out, "%s\n", substr);
	}

	delete[] startaddr;
}

int MaxVertexAtrib()
{
	int a;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &a);

	return a;
}

// **** Log Functions ****

void PrintShaderInfoLog(GLuint shader, char shaderType, char* fileName)
{
	int infologLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 0)
	{
		infoLog = (GLchar*)malloc(infologLen);
		if (infoLog == NULL)
		{
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		
		switch (shaderType)
		{
			case 0: printf("\nVertex");
				break;
			case 1: printf("\nFragment");
				break;
		}

		printf("Shader %s ", fileName);


		printf("InfoLog:\n%s\n\n", infoLog);
		free(infoLog);
	}
}

void PrintProgramInfoLog(GLuint program)
{
	int infologLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 0)
	{
		infoLog = (GLchar*)malloc(infologLen);
		if (infoLog == NULL)
		{
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}
		glGetProgramInfoLog(program, infologLen, &charsWritten, infoLog);
		printf("\nShader Program InfoLog:\n%s\n\n", infoLog);
		free(infoLog);
	}
}


// **** Other Functions ****

char* CopyFileToString(FILE *input, int* length)
{
	int n = 0;
	char *str = (char*)malloc(sizeof(char));
	char c;

	while (TRUE)
	{
		c = fgetc(input);
		str[n] = c;
		n++;
		str = (char*)realloc(str, sizeof(char) * n + sizeof(char));

		if (c == EOF)
			break;
	}

	str[n-1] = '\0';
	*length = n - 1;

	return str;
}

// **** Draw BoundingBox ****

void DrawBoundingBox(BoundingBox &box, glm::vec4 color)
{
	GLuint vao, indexBuffer, vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenVertexArrays(1, &vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* 8, box.vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* 24, box.indices, GL_STATIC_DRAW);


	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	SimpleShader.SetAttribute("World", glm::mat4x4());
	SimpleShader.SetAttribute("UseStaticColor", true);
	SimpleShader.SetAttribute("UseLighting", false);
	SimpleShader.SetAttribute("StaticColor", color);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (GLvoid*)0);
	SimpleShader.SetAttribute("UseStaticColor", false);
	SimpleShader.SetAttribute("UseLighting", true);


	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteVertexArrays(1, &vao);
}

void DrawLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 color)
{
	GLuint vao, vbo;

	glm::vec3 data[2] = { p1, p2 };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3) * 2, data, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(vao);

	SimpleShader.SetAttribute("World", glm::mat4x4());
	SimpleShader.SetAttribute("UseStaticColor", true);
	SimpleShader.SetAttribute("UseLighting", false);
	SimpleShader.SetAttribute("StaticColor", color);
	glDrawArrays(GL_LINES, 0, 2);
	SimpleShader.SetAttribute("UseStaticColor", false);
	SimpleShader.SetAttribute("UseLighting", true);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}