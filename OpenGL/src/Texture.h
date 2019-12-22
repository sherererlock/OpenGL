#pragma once
#include<iostream>

class Texture
{
private:
	unsigned int m_TextureID;
	std::string	m_FilePath;
	unsigned char* m_Buffer;
	int m_Width;
	int m_Height;
	int m_BPP;

public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	const unsigned int GetWidth() const { return m_Width; }
	const unsigned int GetHeight() const { return m_Height; }
	const unsigned int GetBpp() const { return m_Height; }
};