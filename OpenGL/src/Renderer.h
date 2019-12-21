#pragma once

#include"VertexArrays.h"
#include"IndexBuffer.h"
#include"Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* fname, const char* file, int line);

// Render的行为
// 1 接收数据
//	 （1）数据包括： Vertex Array Obect, IndexBuffer, shader
// 2 将数据画在屏幕上
// 3 clear屏幕

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArrays& va, const IndexBuffer& ib, const Shader& shader) const;
};