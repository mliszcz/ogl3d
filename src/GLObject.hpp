/*
 * GLObject.hpp
 *
 *  Created on: Jul 29, 2014
 *      Author: michal
 */

#ifndef GLOBJECT_HPP_
#define GLOBJECT_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

class GLObject {

protected:
	GLuint _handle;

public:
	GLuint handle() {
		return _handle;
	}

	operator GLuint() {
		return _handle;
	}
};

#endif /* GLOBJECT_HPP_ */
