// hello-gl.cpp : Defines the entry point for the application.
//
#include "redbook_lib/include/LoadShaders.h"
#include "redbook_lib/include/vgl.h"
#include <glbinding/glbinding.h>
#include <glbinding/gl45core/gl.h>
#include <print>

using namespace std;

constexpr gl::GLfloat vertices[]{
	.5f,
	.5f,
	.5f,
	-.5f,
	-.5f,
	-.5f,
	-.5f,
	.5f,
};
constexpr gl::GLuint indices[]{
	0,
	1,
	3,
	1,
	2,
	3,
};
struct window_size {
	gl::GLuint x = 800;
	gl::GLuint y = 600;
};

template <gl::GLuint VBsize = 1>
struct gl_state {
	gl::GLuint shader_prog;
	gl::GLuint vao[VBsize];
	gl::GLuint vbo[VBsize];
	gl::GLuint ebo[VBsize];


	gl_state() {
		using namespace gl;
		ShaderInfo shaders[]{
			{ gl::GL_VERTEX_SHADER, "triangle.vert" },
			{ gl::GL_FRAGMENT_SHADER, "triangle.frag" },
			{ gl::GL_NONE, nullptr },
		};
		shader_prog = LoadShaders(shaders);

		glGenVertexArrays(VBsize, vao);
		glGenBuffers(VBsize, vbo);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(vao[0]);

		glBindBuffer(gl::GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, gl::GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	~gl_state() {
		using namespace gl;
		glDeleteVertexArrays(VBsize, vao);
		glDeleteBuffers(VBsize, vbo);
		glDeleteProgram(shader_prog);
	}


	auto display() {
		using namespace gl;
		gl::glClearColor(0, .2, .1, 1);
		gl::glClear(gl::GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_prog);
		glBindVertexArray(vao[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};


#include <GLFW/glfw3.h>
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* window = glfwCreateWindow(800, 600, "Triangles", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		gl::glViewport(0, 0, width, height);
	});

	glbinding::initialize(glfwGetProcAddress);
	gl_state state;
	std::println("Ran Init");
	while (!glfwWindowShouldClose(window)) {
		state.display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
