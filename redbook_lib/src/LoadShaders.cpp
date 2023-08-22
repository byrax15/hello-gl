//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.cxx ---
//
//////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

#include "LoadShaders.h"
#include <glbinding/gl45core/gl.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    //----------------------------------------------------------------------------

    static const gl::GLchar*
        ReadShader(const char* filename)
    {

#ifdef WIN32
        FILE* infile;
        fopen_s(&infile, filename, "rb");
#else
        FILE* infile = fopen(filename, "rb");
#endif // WIN32

        if (!infile) {
#ifdef _DEBUG
            std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
            return NULL;
        }

        fseek(infile, 0, SEEK_END);
        int len = ftell(infile);
        fseek(infile, 0, SEEK_SET);

        gl::GLchar* source = new gl::GLchar[len + 1];

        fread(source, sizeof(gl::GLchar), len, infile);


        fclose(infile);

        source[len] = 0;

        return const_cast<const gl::GLchar*>(source);
    }

    //----------------------------------------------------------------------------

    gl::GLuint
        LoadShaders(ShaderInfo* shaders)
    {
        if (shaders == NULL) { return 0; }

        gl::GLuint program = gl::glCreateProgram();

        ShaderInfo* entry = shaders;
        while (entry->type != gl::GL_NONE) {
            gl::GLuint shader = glCreateShader(entry->type);

            entry->shader = shader;

            const gl::GLchar* source = ReadShader(entry->filename);
            if (source == NULL) {
                for (entry = shaders; entry->type != gl::GL_NONE; ++entry) {
                    gl::glDeleteShader(entry->shader);
                    entry->shader = 0;
                }

                return 0;
            }

            gl::glShaderSource(shader, 1, &source, NULL);
            delete[] source;

            gl::glCompileShader(shader);

            gl::GLint compiled;
            gl::glGetShaderiv(shader, gl::GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
#ifdef _DEBUG
                gl::GLsizei len;
                gl::glGetShaderiv(shader, gl::GL_INFO_LOG_LENGTH, &len);

                gl::GLchar* log = new gl::GLchar[len + 1];
                gl::glGetShaderInfoLog(shader, len, &len, log);
                std::cerr << "Shader compilation failed: " << log << std::endl;
                delete[] log;
#endif /* DEBUG */

                return 0;
            }

            gl::glAttachShader(program, shader);

            ++entry;
        }

        gl::glLinkProgram(program);

        gl::GLint linked;
        gl::glGetProgramiv(program, gl::GL_LINK_STATUS, &linked);
        if (!linked) {
#ifdef _DEBUG
            gl::GLsizei len;
            gl::glGetProgramiv(program, gl::GL_INFO_LOG_LENGTH, &len);

            gl::GLchar* log = new gl::GLchar[len + 1];
            gl::glGetProgramInfoLog(program, len, &len, log);
            std::cerr << "Shader linking failed: " << log << std::endl;
            delete[] log;
#endif /* DEBUG */

            for (entry = shaders; entry->type != gl::GL_NONE; ++entry) {
                gl::glDeleteShader(entry->shader);
                entry->shader = 0;
            }

            return 0;
        }

        return program;
    }

    //----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus