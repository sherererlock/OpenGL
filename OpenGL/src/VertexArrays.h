#pragma once
#include<GL/glew.h>
#include<vector>

#include"VertexBuffer.h"

class VertexBufferLayout;

class VertexArrays
{
private:
	unsigned int m_RendererID;
	
public:
	VertexArrays();
	~VertexArrays();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;
};