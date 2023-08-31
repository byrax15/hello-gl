// hello-gl.cpp : Defines the entry point for the application.
//

#include "Shader.h"
#include "GLstate.h"

#include <glbinding/glbinding.h>
#include <glbinding/gl45core/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL2/SOIL2.h>

#include <span>
#include <array>
#include <algorithm>

using namespace std;

namespace matrix {
	constexpr auto calcProjection = [](float width, float height) {
		return glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	};

	// glm::mat4 model		 = glm::rotate(glm::mat4(1.f), glm::radians(-55.f), glm::vec3{ 1, 0, 0 });
	glm::mat4 model{ 1.f };
	glm::mat4 view		 = glm::translate(glm::mat4(1.f), glm::vec3{ 0, 0, -3.f });
	glm::mat4 projection = calcProjection(800, 600);
}

namespace geometry {
	using ::vertex_uv;

	constexpr std::array vertices{
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, -0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, -0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, -0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, 0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, 0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, 0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, -0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, 0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, -0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, 0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, -0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, -0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, 0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, -0.5f }, glm::vec2{ 1.0f, 1.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ 0.5f, 0.5f, 0.5f }, glm::vec2{ 1.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, 0.5f }, glm::vec2{ 0.0f, 0.0f } },
		vertex_uv{ glm::vec3{ -0.5f, 0.5f, -0.5f }, glm::vec2{ 0.0f, 1.0f } },
	};
	constexpr std::array translations{
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

constexpr auto framebuffer_resize = [](auto*, int width, int height) {
	gl::glViewport(0, 0, width, height);
	matrix::calcProjection(width, height);
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

#include <GLFW/glfw3.h>
constexpr auto exit_app = [](auto* window, int keycode, int, int, int) {
	if (keycode == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
};

#include "GLFWwrapper.h"
int main() {
	GLFWwrapper window;
	glbinding::initialize(glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(*window, framebuffer_resize);
	glfwSetCharCallback(*window, swap_polygon_mode);
	glfwSetKeyCallback(*window, exit_app);

	using GLstate = GLstate<false>;
	Shader triangle{ "shaders/triangle.vs.glsl", "shaders/triangle.fs.glsl" };
	GLstate::init();
	GLstate state{ span{ geometry::vertices }, span{ geometry::indices } };

	auto tex_container = SOIL_load_OGL_texture(
		"textures/container.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);
	auto tex_face = SOIL_load_OGL_texture(
		"textures/awesomeface.png",
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);

	triangle.use();
	triangle.setVec4("baseColor", geometry::blue);
	triangle.setMat4("view", matrix::view);
	triangle.setMat4("projection", matrix::projection);
	triangle.setInt("texture1", 0);
	triangle.setInt("texture2", 1);

	while (!glfwWindowShouldClose(*window)) {
		GLstate::clear_screen();
		gl::glActiveTexture(gl::GL_TEXTURE0);
		gl::glBindTexture(gl::GLenum{ GL_TEXTURE_2D }, tex_container);
		gl::glActiveTexture(gl::GL_TEXTURE1);
		gl::glBindTexture(gl::GLenum{ GL_TEXTURE_2D }, tex_face);

		triangle.use();

		for (size_t i = 0; i < geometry::translations.size(); ++i) {
			matrix::model = glm::mat4(1.f);
			matrix::model = glm::translate(matrix::model, geometry::translations[i]);
			matrix::model = glm::rotate(
				matrix::model,
				(float)glfwGetTime() * glm::radians(20.f * (i + 1)),
				glm::vec3{ .5f, 1.f, .0f });
			triangle.setMat4("model", matrix::model);
			state.draw();
		};

		glfwSwapBuffers(*window);
		glfwPollEvents();
	}
}
