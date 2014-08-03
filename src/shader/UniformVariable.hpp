/*
 * UniformVariable.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: michal
 */

#ifndef UNIFORMVARIABLE_HPP_
#define UNIFORMVARIABLE_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../Common.hpp"
#include "../gfx/type/Matrix.hpp"
#include "../gfx/type/Vector.hpp"
#include "../gfx/type/Scalar.hpp"

namespace shader {

// this is template only to avoid circular dependency
// TProgram is always a Program instance

template <typename TProgram>
class UniformVariable : public GLObject {
	friend class Program;

private:

	UniformVariable(TProgram* program, string name) {
		GLint location = glGetUniformLocation(program->handle(), name.c_str());
		if (location < 0) throw logic_error("cannot find uniform " + name);
		_handle = location;
	}

	template <unsigned int dim, typename T>
	void uniformVector(unsigned int count, const T* data) {
		// no suitable template-specialization found
		throw logic_error("unsupported operation");
	}

	template <unsigned int rows, unsigned int cols, typename T>
	void uniformMatrix(unsigned int count, const T* data, bool transpose) {
		// no suitable template-specialization found
		throw logic_error("unsupported operation");
	}

public:

	UniformVariable& operator=(const glm::mat4& m) {
		glUniformMatrix4fv(_handle, 1, GL_FALSE, glm::value_ptr(m));
		return *this;
	}

	template <unsigned int rows, unsigned int cols, typename T>
	UniformVariable& operator=(const gfx::Matrix<rows, cols, T>& matrix) {
		uniformMatrix<rows, cols, T>(1, matrix.data(), GL_TRUE);
		return *this;
	}

	template <unsigned int dim, typename T>
	UniformVariable& operator=(const gfx::Vector<dim, T>& vect) {
		uniformVector<dim, T>(1, vect.data());
		return *this;
	}

	template <typename T>
	UniformVariable& operator=(const gfx::Scalar<T>& scalar) {
		uniformVector<1, T>(1, scalar.data());
		return *this;
	}

	UniformVariable& operator=(float scalar) {
		uniformVector<1, float>(1, &scalar);
		return *this;
	}

	UniformVariable& operator=(int scalar) {
		uniformVector<1, int>(1, &scalar);
		return *this;
	}

	UniformVariable& operator=(unsigned int scalar) {
		uniformVector<1, unsigned int>(1, &scalar);
		return *this;
	}

	// in case we want to set list of matrices / vectors / scalars ...
	// ... but we DON'T

//	template <unsigned int rows, unsigned int cols, typename T>
//	UniformVariable& operator=(const vector<gfx::Matrix<rows, cols, T>>& data) {
//
//		vector<T> raw(data.size()*rows*cols*sizeof(T), 0);
//
//		typename vector<T>::iterator rawIter = raw.begin();
//		for(auto& matrix : data) {
//			rawIter = std::copy(matrix.data(), matrix.data() + rows*cols, rawIter);
//		}
//
//		uniformMatrix<rows, cols, T>(data.size(), raw.data(), GL_TRUE);
//
//		return *this;
//	}
};

// http://stackoverflow.com/questions/4994775/c-specialization-of-template-function-inside-template-class
// you cannot specialize a templated function of a class template without specializing the class itself...

class Program;

template <> template <>
void UniformVariable<Program>::uniformVector<1, float>(unsigned int count, const float* data) {
	glUniform1fv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<1, int>(unsigned int count, const int* data) {
	glUniform1iv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<1, unsigned int>(unsigned int count, const unsigned int* data) {
	glUniform1uiv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<2, float>(unsigned int count, const float* data) {
	glUniform2fv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<2, int>(unsigned int count, const int* data) {
	glUniform2iv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<2, unsigned int>(unsigned int count, const unsigned int* data) {
	glUniform2uiv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<3, float>(unsigned int count, const float* data) {
	glUniform3fv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<3, int>(unsigned int count, const int* data) {
	glUniform3iv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<3, unsigned int>(unsigned int count, const unsigned int* data) {
	glUniform3uiv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<4, float>(unsigned int count, const float* data) {
	glUniform4fv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<4, int>(unsigned int count, const int* data) {
	glUniform4iv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformVector<4, unsigned int>(unsigned int count, const unsigned int* data) {
	glUniform4uiv(_handle, count, data);
}

template <> template <>
void UniformVariable<Program>::uniformMatrix<2, 2, float>(unsigned int count, const float* data, bool transpose) {
	glUniformMatrix2fv(_handle, count, transpose, data);
}

template <> template <>
void UniformVariable<Program>::uniformMatrix<3, 3, float>(unsigned int count, const float* data, bool transpose) {
	glUniformMatrix3fv(_handle, count, transpose, data);
}

template <> template <>
void UniformVariable<Program>::uniformMatrix<4, 4, float>(unsigned int count, const float* data, bool transpose) {
	glUniformMatrix4fv(_handle, count, transpose, data);
}

}

#endif /* UNIFORMVARIABLE_HPP_ */
