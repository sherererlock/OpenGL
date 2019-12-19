#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<fstream>
#include<sstream>
#include<string>

#include"Renderer.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"

struct ShaderSource
{
	std::string VertexShaderSource;
	std::string PixelShaderSource;
};

static ShaderSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum ShaderType 
	{
		None = -1, Vertex = 0, Pixel = 1,
	};

	std::string line;
	std::stringstream ss[20];

	ShaderType type = ShaderType::None;

	while (getline(stream, line))
	{
		if (line.find("shader")!=std::string::npos)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader =  glCreateShader(type);
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
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vs" : "ps")<<"!" << std::endl;
		std::cout << message << std::endl;

		return 0;
	}

	return shader;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& pixelShader)
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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed\n";
		glfwTerminate();
		return -1;
	}

	float triangles[] =
	{
		0.0f,	-0.5f, // 0
		0.5f,	-0.5f, // 1
		0.25f,	 0.5f, // 2
	};

	unsigned int tindex[] =
	{
		0, 1, 2,
	};

	unsigned int qindex[] =
	{
		0, 1, 2,
		2, 3, 0,
	};

	float position[] =
	{
		-0.5f,	-0.5f, // 0
		0.0f,	-0.5f, // 1
		0.0f,	 0.5f, // 2
		-0.5f,  0.5f, // 3
	};

	// Quad
	unsigned int qvao;
	glGenVertexArrays(1, &qvao);
	glBindVertexArray(qvao);

	VertexBuffer qvb(position, 4 * 2 * sizeof(float));

	GLCall(glEnableVertexAttribArray(0));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // 设置顶点属性的layout，并且与vao绑定

	IndexBuffer qib(qindex, 6);

	// triangle
	unsigned int tvao;
	glGenVertexArrays(1, &tvao);
	glBindVertexArray(tvao);

	VertexBuffer tvb(triangles, 3 * 2 * sizeof(float));

	GLCall(glEnableVertexAttribArray(0));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // 设置顶点属性的layout，并且与vao绑定

	IndexBuffer tib(tindex, 3);

	ShaderSource source = ParseShader("res//shader//Basic.shader");

	unsigned int shaderprogram = CreateShader(source.VertexShaderSource, source.PixelShaderSource);
	glUseProgram(shaderprogram);

	unsigned int location = glGetUniformLocation(shaderprogram, "u_Color");

	//glUniform4f(location, 1.0f, 0.2f, 0.0f, 1.0f);

	float r = 1.0f;
	float increment = 0.01f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Draw Quad
		glBindVertexArray(qvao);
		/*qbuffer.Bind();*/

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		// Draw Triangle
		glBindVertexArray(tvao);

		//tbuffer.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));

		glBindVertexArray(tvao);

		// Uniform
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		if (r >= 1.0f)
			increment = -0.01f;
		else if (r <= 0.0f)
			increment = 0.01f;

		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shaderprogram);

	glfwTerminate();
	return 0;
}