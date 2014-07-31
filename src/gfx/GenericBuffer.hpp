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

private:

	unsigned int _size = 0;

public:

	GenericBuffer() {
		glGenBuffers(1, &_handle);
	}

	~GenericBuffer() {
		glDeleteBuffers(1, &_handle);
	}

	unsigned int size() {
		return _size;
	}

	template <typename T>
	void realloc(const initializer_list<T>& data) {
		realloc(vector<T>(data));
	}

	template <typename T>
	void realloc(const vector<T>& data) {
		glBindBuffer(GL_ARRAY_BUFFER, _handle);
		glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		_size = data.size();
	}
};

}

#endif /* GENERICBUFFER_HPP_ */
