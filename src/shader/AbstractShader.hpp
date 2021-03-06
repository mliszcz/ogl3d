/*
 * AbstractShader.hpp
 *
 *  Created on: Jul 27, 2014
 *      Author: michal
 */

#ifndef ABSTRACTSHADER_HPP_
#define ABSTRACTSHADER_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../Common.hpp"
#include "Shader.hpp"

namespace shader {

template <GLenum shaderType>
class AbstractShader : public Shader {

public:

	static shared_ptr<Shader> fromSource(const string& source) {
		return shared_ptr<Shader>(new AbstractShader(source));
	}

	static shared_ptr<Shader> fromStream(std::istream& stream) {
		std::istreambuf_iterator<char> eos;
		return fromSource(string(std::istreambuf_iterator<char>(stream), eos));
	}

	static shared_ptr<Shader> fromFile(const string& file) {
		std::ifstream fileStream(file);
		return fromStream(fileStream);
	}

	virtual ~AbstractShader() {
		glDeleteShader(_handle);
	}

private:

	AbstractShader(string source) {

		_handle = glCreateShader(shaderType);
		const char* szSource = source.c_str();
		glShaderSource(_handle, 1, &szSource, nullptr);
		glCompileShader(_handle);

		if (getShaderiv(_handle, GL_COMPILE_STATUS) == GL_FALSE) {
			throw logic_error("failed to compile the shader:\n" + getShaderInfoLog(_handle) + "\nsource\n" + source);
		}
	}
};

}

#endif /* ABSTRACTSHADER_HPP_ */
