/*
 * Common.hpp
 *
 *  Created on: Jul 26, 2014
 *      Author: michal
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

/*
 * GLM: angleAxis function taking degrees as a parameter is deprecated.
 * #define GLM_FORCE_RADIANS before including GLM headers to remove this message.
 */
#define GLM_FORCE_RADIANS

#include <initializer_list>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <list>
#include <map>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::pair;
using std::stack;
using std::list;
using std::map;
using std::function;
using std::initializer_list;

using std::logic_error;
using std::length_error;

#include "GLObject.hpp"


namespace util {

inline void CheckError() {
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		const char* errorString = reinterpret_cast<const char*>(gluErrorString(errorCode));
		throw logic_error(string("OpenGL error: ") + string(errorString));
	}
}

inline float DegToRad(float deg) {
	return deg * 3.1415f / 180.0f;
}

inline float RadToDeg(float rad) {
	return rad * 180.0f / 3.1415f;
}

}

#endif /* COMMON_HPP_ */
