/*
 * Matrix.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: michal
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include "../../Common.hpp"

namespace gfx {

template <unsigned int rows, unsigned int cols, typename T>
class Matrix {

private:
	vector<T> _data;

public:

	Matrix(const vector<T>& data) {
		if (data.size() != rows*cols) throw length_error("invalid length");
		_data = data;
	}

	Matrix(const initializer_list<T>& data)
		: Matrix(vector<T>(data)) { }

	Matrix()
		: Matrix(vector<T>(rows*cols, 0)) { }

	virtual ~Matrix() = default;

	virtual T& at(unsigned int row, unsigned int col) {
		return _data.at(row*cols+col);
	}

	T* data() {
		return _data.data();
	}
};

}

#endif /* MATRIX_HPP_ */
