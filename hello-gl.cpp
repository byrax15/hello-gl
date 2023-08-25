// hello-gl.cpp : Defines the entry point for the application.
//
#include <glbinding/glbinding.h>
#include <glbinding/gl45core/gl.h>
#include <span>
#include <array>
#include <bit>
#include <numbers>
#include "Shader.h"
#include "GLstate.h"


using namespace std;


static constexpr std::array vertices{
	vertex{ -.5, -.5, 0 },
	vertex{ .5, -.5, 0 },
	vertex{ 0, .5, 0 },
};
static constexpr std::array<gl::GLfloat, 4> red{ 1, 0, 0, 1 };


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
#include <cassert>
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


	Shader	triangle{ "triangle.vs.glsl", "triangle.fs.glsl" };
	GLstate state{ triangle, span{ vertices } };

	while (!glfwWindowShouldClose(window)) {
		GLstate::clear_screen();
		triangle.use();

		const auto time = glfwGetTime();
		triangle.setFloat("time", std::cosf(time) + 1);
		triangle.setVec4("color", span{ red });

		state.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
