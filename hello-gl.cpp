// hello-gl.cpp : Defines the entry point for the application.
//
#include <gl_abstractions.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl45core/gl.h>
#include <print>
#include <span>


using namespace std;
using namespace gl_abstractions;


constexpr vec3 vertices[]{
	{ .5f, .5f, 0 },
	{ .5f, -.5f, 0 },
	{ -.5f, -.5f, 0 },
	{ -.5f, .5f, 0 }
};
constexpr triangle tri1[]{ { 0, 1, 3 } };
constexpr triangle tri2[]{ { 1, 2, 3 } };

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
	gl_job jobs[]{
		{ "triangle.vert", "triangle_mauve.frag", span{ tri1 }, span{ vertices } },
		{ "triangle.vert", "triangle_orange.frag", span{ tri2 }, span{ vertices } },
	};

	std::println("Ran Init");
	while (!glfwWindowShouldClose(window)) {
		gl_job::clear();
		for (auto& j : jobs)
			j.display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
