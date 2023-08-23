#include <LoadShaders.h>
#include <vgl.h>
#include <glbinding/gl45core/types.h>
#include <span>
#include <array>

namespace gl_abstractions {

	template <gl::GLuint Size>
	struct vec {
		std::array<gl::GLfloat, Size> data;
	};
	using vec3 = vec<3>;

	struct triangle {
		gl::GLuint vertex_id[3];
	};


	struct gl_job {
		gl::GLuint shader_prog;
		gl::GLuint vao, vbo, ebo;

		gl_job(
			const char* vertShaderName,
			const char* fragShaderName,
			std::span<const triangle> triangles,
			std::span<const vec3> vertices);
		~gl_job();
		auto display() -> void;
		static auto clear() -> void;
	};
}
