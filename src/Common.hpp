/*
 * Common.hpp
 *
 *  Created on: Jul 26, 2014
 *      Author: michal
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <iterator>
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
using std::stack;
using std::list;
using std::map;
using std::initializer_list;

using std::logic_error;
using std::length_error;

#include "GLObject.hpp"
//#include "gfx/type/Matrix.hpp"


namespace util {

inline void CheckError() {
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		const char* errorString = reinterpret_cast<const char*>(gluErrorString(errorCode));
		throw logic_error(string("OpenGL error: ") + string(errorString));
	}
}

}

#endif /* COMMON_HPP_ */
