#include<GL/glew.h>
#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string & filepath)
	: m_FilePath(filepath)
{
	ShaderSource source = ParseShader();
	m_ProgramID = CreateShader(source.VertexShaderSource, source.PixelShaderSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_ProgramID));
}

ShaderSource Shader::ParseShader()
{
	std::ifstream stream(m_FilePath);

	enum ShaderType
	{
		None = -1, Vertex = 0, Pixel = 1,
	};

	std::string line;
	std::stringstream ss[20];

	ShaderType type = ShaderType::None;

	while (getline(stream, line))
	{
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::Vertex;
			else if (line.find("pixel") != std::string::npos)
				type = ShaderType::Pixel;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	// TODO error handle.
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vs" : "ps") << "!" << std::endl;
		std::cout << message << std::endl;

		return 0;
	}

	return shader;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& pixelShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int ps = CompileShader(GL_FRAGMENT_SHADER, pixelShader);

	glAttachShader(program, vs);
	glAttachShader(program, ps);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(ps);

	return program;
}

int Shader::GetUniformLocation(const std::string &name)
{
	GLCall(int location = glGetUniformLocation(m_ProgramID, name.c_str()));
	if (location == -1)
		std::cout << "[OpenGL Warning] Can't find location for " << name << "!\n";

	return location;
}

void Shader::SetValue(const std::string& name, float v0, float v1, float v2, float v3)
{
	// Uniform
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_ProgramID));
}

void Shader::UnBind() const
{
	GLCall(glUseProgram(0));
}

