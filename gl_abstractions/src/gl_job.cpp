#include "gl_abstractions.h"
#include <glbinding/gl45core/gl.h>

gl_abstractions::gl_job::gl_job(
	const char* vertShaderName,
	const char* fragShaderName,
	std::span<const triangle> triangles,
	std::span<const vec3> vertices) {
	ShaderInfo shaders[]{
		{ gl::GL_VERTEX_SHADER, vertShaderName },
		{ gl::GL_FRAGMENT_SHADER, fragShaderName },
		{ gl::GL_NONE, nullptr },
	};
	shader_prog = LoadShaders(shaders);

	using namespace gl;
	gl::glGenVertexArrays(1, &vao);
	gl::glGenBuffers(1, &vbo);
	gl::glGenBuffers(1, &ebo);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	gl::glBindVertexArray(vao);

	gl::glBindBuffer(GL_ARRAY_BUFFER, vbo);
	gl::glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

	gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size_bytes(), triangles.data(), GL_STATIC_DRAW);

	gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(triangle), BUFFER_OFFSET(0));
	gl::glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	gl::glBindBuffer(GL_ARRAY_BUFFER, 0);
}


gl_abstractions::gl_job::~gl_job() {
	gl::glDeleteVertexArrays(1, &vao);
	gl::glDeleteBuffers(1, &vbo);
	gl::glDeleteBuffers(1, &ebo);
	gl::glDeleteProgram(shader_prog);
}


auto gl_abstractions::gl_job::display() -> void {
	gl::glUseProgram(shader_prog);
	// seeing as we only have a single VAO there's no need to bind it every time,
	// but we'll do so to keep things a bit more organized
	gl::glBindVertexArray(vao);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	gl::glDrawElements(gl::GL_TRIANGLES, 3, gl::GL_UNSIGNED_INT, 0);
}


auto gl_abstractions::gl_job::clear() -> void{
	gl::glClearColor(0, .2, .1, 1);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT);
}
