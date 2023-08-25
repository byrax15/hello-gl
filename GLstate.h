#pragma once
#include <glbinding/gl45core/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"

template <bool IndexedRendering = false>
struct GLstate {
	gl::GLuint vao;
	gl::GLuint vbo;
	gl::GLuint ebo;
	gl::GLuint indicesCount;
	Shader	   shader;

	explicit GLstate(
		Shader						shader,
		std::span<const glm::vec3>	vertices,
		std::span<const glm::uvec3> indices)
		: shader(shader),
		  indicesCount(indices.size() * glm::uvec3::length()) {

		using namespace gl;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

		if constexpr (IndexedRendering) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);
		}

		glVertexAttribPointer(0, glm::vec3::length(), GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		// glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	auto draw() -> void {
		using namespace gl;
		glBindVertexArray(vao);
		if constexpr (IndexedRendering) {
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	static auto clear_screen() -> void {
		gl::glClearColor(0, .2, .1, 1);
		gl::glClear(gl::GL_COLOR_BUFFER_BIT);
	}

	~GLstate() {
		using namespace gl;
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
};
