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
	GLuint handle;

public:
	GLuint getHandle() {
		return handle;
	}

	operator GLuint() {
		return handle;
	}
};

#endif /* GLOBJECT_HPP_ */
