#include "VertexArrays.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArrays::VertexArrays()
{
	glGenVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
}

VertexArrays::~VertexArrays()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArrays::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		GLCall(glEnableVertexAttribArray(i));
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(),(const void*)offset); // ���ö������Ե�layout��������vao��
		offset += elements[i].count * VertexBufferElement::GetSizeOf(elements[i].type);
	}
}

void VertexArrays::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArrays::UnBind() const
{
	glBindVertexArray(0);
}

