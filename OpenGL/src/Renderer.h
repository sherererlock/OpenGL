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

// Render����Ϊ
// 1 ��������
//	 ��1�����ݰ����� Vertex Array Obect, IndexBuffer, shader
// 2 �����ݻ�����Ļ��
// 3 clear��Ļ

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArrays& va, const IndexBuffer& ib, const Shader& shader) const;
};