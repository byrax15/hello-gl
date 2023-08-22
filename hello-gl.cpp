// hello-gl.cpp : Defines the entry point for the application.
//
#include "redbook_lib/include/LoadShaders.h"
#include "redbook_lib/include/vgl.h"
#include <glbinding/glbinding.h>
#include <glbinding/gl45core/gl.h>
#include <GLFW/glfw3.h>
#include <print>

enum VAO_IDs { Triangles, NumVAOs, };
enum Buffer_IDs { ArrayBuffer, NumBuffers, };
enum Attrib_IDs { vPosition = 0, };

gl::GLuint VAOs[NumVAOs];
gl::GLuint Buffers[NumBuffers];

constexpr gl::GLuint NumVertices = 6;

auto init()
{
	gl::glGenVertexArrays(NumVAOs, VAOs);
	gl::glBindVertexArray(VAOs[Triangles]);

	static const GLfloat vertices[NumVertices][2] = {
		{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
		{  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f },  // Triangle 2
	};

	gl::glCreateBuffers(NumBuffers, Buffers);
	gl::glBindBuffer(gl::GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	gl::glBufferStorage(gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, gl::BufferStorageMask{ 0 });

	ShaderInfo shaders[] =
	{
		{ gl::GL_VERTEX_SHADER, "triangle.vert" },
		{ gl::GL_FRAGMENT_SHADER, "triangle.frag" },
		{ gl::GLenum{ GL_NONE } , NULL}
	};

	gl::GLuint program = LoadShaders(shaders);
	gl::glUseProgram(program);

	gl::glVertexAttribPointer(vPosition, 2, gl::GLenum{ GL_FLOAT }, GL_FALSE, 0, BUFFER_OFFSET(0));
	gl::glEnableVertexAttribArray(vPosition);
}

auto display()
{
	constexpr gl::GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	gl::glClearBufferfv(gl::GLenum{ GL_COLOR }, 0, black);
	gl::glBindVertexArray(VAOs[Triangles]);
	gl::glDrawArrays(gl::GLenum{ GL_TRIANGLES }, 0, NumVertices);
}

#include <GLFW/glfw3.h>
int main()
{
	glfwInit();
	auto* window = glfwCreateWindow(800, 600, "Triangles", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glbinding::initialize(glfwGetProcAddress);

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
