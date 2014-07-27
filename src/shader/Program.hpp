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

namespace shader {

class Program {

private:

	GLuint handle;

public:

	GLuint getHandle() {
		return handle;
	}

	Program(const vector<shared_ptr<Shader>>& shaders) {

		handle = glCreateProgram();

		for(auto& shader : shaders)
			glAttachShader(handle, shader->getHandle());

		glLinkProgram(handle);

		if (getProgramiv(handle, GL_LINK_STATUS) == GL_FALSE) {
			util::Logger::getInstance()->
					error("failed to link the program:\n%s\n", getProgramInfoLog(handle).c_str());
		}

		for(auto& shader : shaders)
			glDetachShader(handle, shader->getHandle());
	}

	~Program() {
		glDeleteProgram(handle);
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
