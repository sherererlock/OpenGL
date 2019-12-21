#pragma once

// shader的行为：
// 1 从文件中读取shader字符串， 解析字符串
// 2 编译shader
// 3 链接shader
// 4 使用shader
// 5 设置uniform
// shader需要的变量:
// 过程中的量:
// 1 字符串结构体
// 2 shader的ID

// 需要保存的变量:
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