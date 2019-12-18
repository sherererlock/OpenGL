#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<fstream>
#include<sstream>
#include<string>

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

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed\n";
		glfwTerminate();
		return -1;
	}

	float position[] =
	{
		-0.5f,	-0.5f, // 0
		 0.5f,	-0.5f, // 1
		 0.5f,	 0.5f, // 2
		 -0.5f,  0.5f, // 3
	};

	unsigned int index[] =
	{
		0, 1, 2,
		2, 3, 0,
	};

	unsigned int bufferid;
	glGenBuffers(1, &bufferid);
	glBindBuffer(GL_ARRAY_BUFFER, bufferid);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), position, GL_STATIC_DRAW);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index, GL_STATIC_DRAW);

	ShaderSource source = ParseShader("res//shader//Basic.shader");

	unsigned int shaderprogram = CreateShader(source.VertexShaderSource, source.PixelShaderSource);
	glUseProgram(shaderprogram);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shaderprogram);

	glfwTerminate();
	return 0;
}