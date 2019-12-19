#include<iostream>
#include<GL/glew.h>

#include"Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* fname, const char* file, int line)
{
	if (GLenum error = glGetError() != GL_NO_ERROR)
	{
		std::cout << "[OpenGL Error](" << error << "):" << fname << " " << file << " :" << line << std::endl;
		return false;
	}

	return true;
}