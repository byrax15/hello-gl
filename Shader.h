#pragma once

#include <glbinding/gl45core/gl.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <span>

class Shader {
public:
	unsigned int ID;
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath) {
		using namespace gl;
		// 1. retrieve the vertex/fragment source code from filePath
		std::string	  vertexCode;
		std::string	  fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode	 = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
#ifndef NDEBUG
		checkCompileErrors(vertex, "VERTEX");
#endif
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
#ifndef NDEBUG
		checkCompileErrors(fragment, "FRAGMENT");
#endif
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
#ifndef NDEBUG
		checkCompileErrors(ID, "PROGRAM");
#endif
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use() {
		gl::glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string_view name, bool value) const {
		gl::glUniform1i(gl::glGetUniformLocation(ID, name.data()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string_view name, int value) const {
		gl::glUniform1i(gl::glGetUniformLocation(ID, name.data()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string_view name, float value) const {
		gl::glUniform1f(gl::glGetUniformLocation(ID, name.data()), value);
	}

	void setVec4(const std::string_view name, std::span<const gl::GLfloat, 4> vec) const {
		auto loc = gl::glGetUniformLocation(ID, name.data());
		gl::glUniform4fv(loc, 1, vec.data());
	}

private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type) {
		using namespace gl;
		int	 success;
		char infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
						  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
						  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
