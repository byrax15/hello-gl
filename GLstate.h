#pragma once
#include <glbinding/gl45core/gl.h>
#include "Shader.h"

struct vertex {
	std::array<gl::GLfloat, 3> coords;
};


struct GLstate {
	gl::GLuint vao;
	gl::GLuint vbo;
	Shader	   shader;

	explicit GLstate(
		Shader					shader,
		std::span<const vertex> vertices)
		: shader(shader) {

		using namespace gl;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
		glEnableVertexAttribArray(0);
	}

	auto draw() -> void {
		using namespace gl;
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	static auto clear_screen() -> void {
		gl::glClearColor(0, .2, .1, 1);
		gl::glClear(gl::GL_COLOR_BUFFER_BIT);
	}
};
