/*
 * Vector.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: michal
 */

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include "Matrix.hpp"
#include "../../Common.hpp"

namespace gfx {

//[[deprecated("use glm instead")]]
template <unsigned int dim, typename T>
class Vector : public Matrix<dim, 1, T> {

public:

	Vector(const vector<T>& data)
		: Matrix<dim, 1, T>(data) { }

	Vector(const initializer_list<T>& data)
		: Matrix<dim, 1, T>(data) { }

	Vector()
		: Matrix<dim, 1, T>() { }

	virtual ~Vector() = default;

	virtual T& at(unsigned int pos) {
		return Matrix<dim, 1, T>::at(pos, 0);
	}
};

}

#endif /* VECTOR_HPP_ */
