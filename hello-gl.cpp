// hello-gl.cpp : Defines the entry point for the application.
//
#include <glbinding/glbinding.h>
#include <glbinding/gl45core/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <span>
#include <array>
#include "Shader.h"
#include "GLstate.h"

using namespace std;

constexpr  auto calcProjection = [](float width, float height) {
	return glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
};

// glm::mat4 model		 = glm::rotate(glm::mat4(1.f), glm::radians(-55.f), glm::vec3{ 1, 0, 0 });
glm::mat4 model{ 1.f };
glm::mat4 view		 = glm::translate(glm::mat4(1.f), glm::vec3{ 0, 0, -3.f });
glm::mat4 projection = calcProjection(800, 600);

namespace gl_callbacks {

	constexpr auto framebuffer_resize = [](auto*, int width, int height) {
		gl::glViewport(0, 0, width, height);
		calcProjection(800, 600);
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

namespace geometry {

	static constexpr std::array vertices{
		glm::vec3{ -0.5f, -0.5f, -0.5f },
		glm::vec3{ 0.5f, -0.5f, -0.5f },
		glm::vec3{ 0.5f, 0.5f, -0.5f },
		glm::vec3{ 0.5f, 0.5f, -0.5f },
		glm::vec3{ -0.5f, 0.5f, -0.5f },
		glm::vec3{ -0.5f, -0.5f, -0.5f },
		glm::vec3{ -0.5f, -0.5f, 0.5f },
		glm::vec3{ 0.5f, -0.5f, 0.5f },
		glm::vec3{ 0.5f, 0.5f, 0.5f },
		glm::vec3{ 0.5f, 0.5f, 0.5f },
		glm::vec3{ -0.5f, 0.5f, 0.5f },
		glm::vec3{ -0.5f, -0.5f, 0.5f },
		glm::vec3{ -0.5f, 0.5f, 0.5f },
		glm::vec3{ -0.5f, 0.5f, -0.5f },
		glm::vec3{ -0.5f, -0.5f, -0.5f },
		glm::vec3{ -0.5f, -0.5f, -0.5f },
		glm::vec3{ -0.5f, -0.5f, 0.5f },
		glm::vec3{ -0.5f, 0.5f, 0.5f },
		glm::vec3{ 0.5f, 0.5f, 0.5f },
		glm::vec3{ 0.5f, 0.5f, -0.5f },
		glm::vec3{ 0.5f, -0.5f, -0.5f },
		glm::vec3{ 0.5f, -0.5f, -0.5f },
		glm::vec3{ 0.5f, -0.5f, 0.5f },
		glm::vec3{ 0.5f, 0.5f, 0.5f },
		glm::vec3{ -0.5f, -0.5f, -0.5f },
		glm::vec3{ 0.5f, -0.5f, -0.5f },
		glm::vec3{ 0.5f, -0.5f, 0.5f },
		glm::vec3{ 0.5f, -0.5f, 0.5f },
		glm::vec3{ -0.5f, -0.5f, 0.5f },
		glm::vec3{ -0.5f, -0.5f, -0.5f },
		glm::vec3{ -0.5f, 0.5f, -0.5f },
		glm::vec3{ 0.5f, 0.5f, -0.5f },
		glm::vec3{ 0.5f, 0.5f, 0.5f },
		glm::vec3{ 0.5f, 0.5f, 0.5f },
		glm::vec3{ -0.5f, 0.5f, 0.5f },
		glm::vec3{ -0.5f, 0.5f, -0.5f },
	};
	static constexpr std::array translations{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f),

	};
	static constexpr std::array indices{
		glm::uvec3{ 0, 1, 2 },
		glm::uvec3{ 0, 2, 3 },
	};
	static constexpr glm::vec4 red{ 1, 0, 0, 1 };
	static constexpr glm::vec4 blue{ 0, 0, 1, 1 };
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


	using GLstate = GLstate<false>;
	glbinding::initialize(glfwGetProcAddress);
	Shader triangle{ "triangle.vs.glsl", "triangle.fs.glsl" };
	GLstate::init();
	GLstate state{ triangle, span{ geometry::vertices }, span{ geometry::indices } };

	while (!glfwWindowShouldClose(window)) {
		GLstate::clear_screen();

		triangle.use();
		triangle.setVec4("baseColor", geometry::blue);
		triangle.setMat4("view", view);
		triangle.setMat4("projection", projection);


		for (size_t i = 0; i < geometry::translations.size(); ++i) {
			model = glm::mat4(1.f);
			model = glm::translate(model, geometry::translations[i]);
			model = glm::rotate(
				model,
				(float)glfwGetTime() * glm::radians(20.f * (i + 1)),
				glm::vec3{ .5f, 1.f, .0f });
			triangle.setMat4("model", model);
			state.draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}
