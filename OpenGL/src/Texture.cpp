#include "Texture.h"
#include "Renderer.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& filepath)
	: m_TextureID(0),m_FilePath(filepath),m_Buffer(nullptr),m_Width(0),m_Height(0),m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);

	GLCall(glGenTextures(1, &m_TextureID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
	m_Buffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_Buffer)
		stbi_image_free(m_Buffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_TextureID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
