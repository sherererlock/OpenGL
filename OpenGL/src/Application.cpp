#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

#include"Renderer.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"VertexArrays.h"
#include"Shader.h"
#include"VertexBufferLayout.h"
#include"Texture.h" 

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
		0.0f,	-0.5f, 1.0f, 0.0f,// 0
		0.5f,	-0.5f, 0.0f, 0.0f,// 1
		0.25f,	 0.5f, 0.5f, 1.0f,// 2
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
		-0.5f,	-0.5f, 0.0f, 0.0f,// 0
		0.0f,	-0.5f, 1.0f, 0.0f,// 1
		0.0f,	 0.5f, 1.0f, 1.0f,// 2
		-0.5f,  0.5f,  0.0f, 1.0f// 3
	};

	{

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		Renderer renderer;

		VertexArrays va;
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		// Quad
		VertexBuffer qvb(position, 4 * 4 * sizeof(float));
		IndexBuffer qib(qindex, 6);

		// Triangle
		VertexBuffer tvb(triangles, 3 * 4 * sizeof(float));
		IndexBuffer tib(tindex, 3);

		Shader shader("res//shader//Basic.shader");
		shader.Bind();

		Texture texture1("res//textures//test.jpg");
		texture1.Bind();

		Texture texture2("res//textures//test2.jpg");

		shader.SetValue("u_Texture", 0);
		shader.SetValue("u_Color", 0.0f, 0.0f, 0.0f, 0.0f);

		va.UnBind();
		qvb.UnBind();
		tvb.UnBind();
		tib.UnBind();
		shader.UnBind();

		float r = 1.0f;
		float increment = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			// Uniform
			shader.Bind();

			// Draw Quad
			texture1.Bind();
			va.AddBuffer(qvb, layout);
			renderer.Draw(va, qib, shader);

			// Draw Triangle
			texture2.Bind();
			va.AddBuffer(tvb, layout);
			renderer.Draw(va, tib, shader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();

	return 0;
}