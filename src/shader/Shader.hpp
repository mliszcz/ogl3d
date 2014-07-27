/*
 * Shader.hpp
 *
 *  Created on: Jul 26, 2014
 *      Author: michal
 */

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../Common.hpp"

namespace shader {

class Shader {

protected:
	GLuint handle;

public:
	GLuint getHandle() {
		return handle;
	}

protected:

	virtual ~Shader() = 0;

	GLint getShaderiv(GLuint shader, GLenum pname) {
		GLint result;
		glGetShaderiv(shader, pname, &result);
		return result;
	}

	string getShaderInfoLog(GLuint shader) {
		GLint len = getShaderiv(shader, GL_INFO_LOG_LENGTH);
		string log(len, 0);
		glGetShaderInfoLog(shader, len, nullptr, &log[0]);
		return log;
	}
};

inline Shader::~Shader() { }

}

#endif /* SHADER_HPP_ */
