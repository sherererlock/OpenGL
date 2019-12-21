#pragma once

// shader����Ϊ��
// 1 ���ļ��ж�ȡshader�ַ����� �����ַ���
// 2 ����shader
// 3 ����shader
// 4 ʹ��shader
// 5 ����uniform
// shader��Ҫ�ı���:
// �����е���:
// 1 �ַ����ṹ��
// 2 shader��ID

// ��Ҫ����ı���:
// 1 filepath
// 2 programID

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

struct ShaderSource
{
	std::string VertexShaderSource;
	std::string PixelShaderSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_ProgramID;

private:
	ShaderSource ParseShader();
	unsigned int CreateShader(const std::string& vertexShader, const std::string& pixelShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);

public:
	Shader(const std::string& filepath);
	~Shader();

	void SetValue(const std::string& name, float v0, float v1, float v2, float v3);

	void Bind() const;
	void UnBind() const;
};