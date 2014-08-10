/*
 * Program.hpp
 *
 *  Created on: Jul 27, 2014
 *      Author: michal
 */

#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shader.hpp"
#include "../Common.hpp"
#include "UniformVariable.hpp"

namespace shader {

class Program : public GLObject {

public:

	UniformVariable<Program> uniform(string name) {
		return UniformVariable<Program>(this, name);
	}

	Program(const vector<shared_ptr<Shader>>& shaders) {

		_handle = glCreateProgram();

		for(auto& shader : shaders)
			glAttachShader(_handle, *shader);

		glLinkProgram(_handle);

		if (getProgramiv(_handle, GL_LINK_STATUS) == GL_FALSE) {
			throw logic_error("failed to link the program:\n" + getProgramInfoLog(_handle));
		}

		for(auto& shader : shaders)
			glDetachShader(_handle, *shader);
	}

	Program(const initializer_list<shared_ptr<Shader>>& shaders)
		: Program(vector<shared_ptr<Shader>>(shaders)) { }

	~Program() {
		glDeleteProgram(_handle);
	}

	void use() {
		glUseProgram(_handle);
	}

	void dispose() {
		glUseProgram(0);
	}

	void bind() {
		glUseProgram(_handle);
	}

	void unbind() {
		glUseProgram(0);
	}

private:

	GLint getProgramiv(GLuint program, GLenum pname) {
		GLint result;
		glGetProgramiv(program, pname, &result);
		return result;
	}

	string getProgramInfoLog(GLuint program) {
		GLint len = getProgramiv(program, GL_INFO_LOG_LENGTH);
		string log(len, 0);
		glGetProgramInfoLog(program, len, nullptr, &log[0]);
		return log;
	}
};

}

#endif /* PROGRAM_HPP_ */
