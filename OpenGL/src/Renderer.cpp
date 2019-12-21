#include<iostream>
#include<GL/glew.h>

#include"Renderer.h"
#include"VertexArrays.h"
#include"IndexBuffer.h"
#include"Shader.h"

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

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArrays & va, const IndexBuffer & ib, const Shader & shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
