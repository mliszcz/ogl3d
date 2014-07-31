/*
 * Scalar.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: michal
 */

#ifndef SCALAR_HPP_
#define SCALAR_HPP_

#include "Vector.hpp"
#include "../../Common.hpp"

namespace gfx {

template <typename T>
class Scalar : public Vector<1, T> {

public:

	Scalar(T t)
		: Vector<1, T>(vector<T>(1, t)) { }

	Scalar()
		: Vector<1, T>() { }

	virtual ~Scalar() = default;

	operator T&() {
		return Vector<1, T>::at(0);
	}
};

}

#endif /* SCALAR_HPP_ */
