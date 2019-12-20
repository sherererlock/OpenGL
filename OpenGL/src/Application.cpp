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

	{
		VertexArrays va;
		VertexBufferLayout layout;
		layout.Push<float>(2);

		// Quad
		VertexBuffer qvb(position, 4 * 2 * sizeof(float));
		IndexBuffer qib(qindex, 6);

		// Triangle
		VertexBuffer tvb(triangles, 3 * 2 * sizeof(float));
		IndexBuffer tib(tindex, 3);

		Shader shader("res//shader//Basic.shader");
		shader.UnBind();

		va.UnBind();
		qvb.UnBind();
		tvb.UnBind();
		tib.UnBind();

		float r = 1.0f;
		float increment = 0.01f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			// Uniform
			shader.Bind();
			shader.SetValue("u_Color", r, 0.3f, 0.8f, 1.0f);

			// Draw Quad
			va.AddBuffer(qvb, layout);
			qib.Bind();
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			// Draw Triangle
			va.AddBuffer(tvb, layout);
			tib.Bind();
			GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));

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
	}

	glfwTerminate();

	return 0;
}