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
	0,
	.5f,
	-.5f,
	0,
	-.5f,
	-.5f,
	0,
	-.5f,
	.5f,
	0,
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
		ShaderInfo shaders[]{
			{ gl::GL_VERTEX_SHADER, "triangle.vert" },
			{ gl::GL_FRAGMENT_SHADER, "triangle.frag" },
			{ gl::GL_NONE, nullptr },
		};
		shader_prog = LoadShaders(shaders);

		using namespace gl;
		gl::glGenVertexArrays(VBsize, vao);
		gl::glGenBuffers(VBsize, vbo);
		gl::glGenBuffers(VBsize, ebo);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		gl::glBindVertexArray(vao[0]);

		gl::glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		gl::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
		gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		gl::glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		gl::glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	~gl_state() {
		gl::glDeleteVertexArrays(VBsize, vao);
		gl::glDeleteBuffers(VBsize, vbo);
		gl::glDeleteBuffers(VBsize, ebo);
		gl::glDeleteProgram(shader_prog);
	}


	auto display() {
		gl::glClearColor(0, .2, .1, 1);
		gl::glClear(gl::GL_COLOR_BUFFER_BIT);

		gl::glUseProgram(shader_prog);
		// seeing as we only have a single VAO there's no need to bind it every time,
		// but we'll do so to keep things a bit more organized
		gl::glBindVertexArray(vao[0]);
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		gl::glDrawElements(gl::GL_TRIANGLES, 6, gl::GL_UNSIGNED_INT, 0);
	}
};

namespace gl_callbacks {
	constexpr auto framebuffer_resize = [](auto*, int width, int height) {
		gl::glViewport(0, 0, width, height);
	};

	constexpr auto swap_polygon_mode = [](auto*, gl::GLuint input) {
		gl::GLenum mode = gl::GL_LINE;
		switch (input) {
		case 'd':
			mode = gl::GL_LINE;
			break;
		case 'f':
			mode = gl::GL_FILL;
			break;
		default:
			return;
		}
		gl::glPolygonMode(gl::GL_FRONT_AND_BACK, mode);
	};
}

#include <GLFW/glfw3.h>
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto* window = glfwCreateWindow(800, 600, "Triangles", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, gl_callbacks::framebuffer_resize);
	glfwSetCharCallback(window, gl_callbacks::swap_polygon_mode);
	glfwSetKeyCallback(window, [](auto* window, int keycode, int, int, int) {
		if (keycode == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
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
