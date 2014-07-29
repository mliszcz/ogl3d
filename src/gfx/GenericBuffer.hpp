/*
 * GenericBuffer.hpp
 *
 *  Created on: Jul 29, 2014
 *      Author: michal
 */

#ifndef GENERICBUFFER_HPP_
#define GENERICBUFFER_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../Common.hpp"

namespace gfx {

class GenericBuffer : public GLObject {

public:

	GenericBuffer() {
		glGenBuffers(1, &handle);
	}

	~GenericBuffer() {
		glDeleteBuffers(1, &handle);
	}

	template <typename T>
	void realloc(const initializer_list<T>& data) {
		realloc(vector<T>(data));
	}

	template <typename T>
	void realloc(const vector<T>& data) {
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

}

#endif /* GENERICBUFFER_HPP_ */
