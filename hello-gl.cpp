// hello-gl.cpp : Defines the entry point for the application.
//
#include <print>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "redbook_lib/include/LoadShaders.h"
#include "redbook_lib/include/vgl.h"

enum VAO_IDs { Triangles, NumVAOs, };
enum Buffer_IDs { ArrayBuffer, NumBuffers, };
enum Attrib_IDs { vPosition = 0, };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

constexpr GLuint NumVertices = 6;

auto init()
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	constexpr GLfloat vertices[NumVertices][2] = {
		{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f },  // Triangle 2
	};

	glCreateBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "triangle.vert" },
		{ GL_FRAGMENT_SHADER, "triangle.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
}

auto display()
{
	constexpr GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}


int main()
{
	glfwInit();
	auto* window = glfwCreateWindow(800, 600, "Triangles", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (gl3wInit()) {
		std::println(stderr, "Can't init OpenGL");
		return -10;
	}

	if (!gl3wIsSupported(4, 5)) {
		std::println(stderr, "OpenGL driver must support version greater or equal to 4.5");
		return -20;
	}

	init();
	std::println("Ran Init");
	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
