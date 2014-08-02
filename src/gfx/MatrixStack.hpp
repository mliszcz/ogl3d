/*
 * MatrixStack.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: michal
 */

#ifndef MATRIXSTACK_HPP_
#define MATRIXSTACK_HPP_

#include "../Common.hpp"
#include "type/Matrix.hpp"

namespace gfx {

template <unsigned int rows, unsigned int cols, typename T>
class MatrixStack {

	typedef Matrix<rows, cols, T> TMatrix;

private:

	stack<TMatrix> matrices;
	TMatrix current = TMatrix::Identity();

public:

	const TMatrix& top() {
		return current;
	}

	void push() {
		matrices.push(current);
	}

	void pop() {
		current = matrices.top();
		matrices.pop();
	}

	void mul(const TMatrix& matrix) {
		if (rows != cols) throw logic_error("non-square matrix multiplication");
		TMatrix tmp;

		TMatrix& matrix_nc = const_cast<TMatrix&>(matrix);

		for (int i=0; i<rows; i++)
			for (int j=0; j<cols; j++)
				for (int k=0; k <rows; ++k)
					tmp.at(i,j) += current.at(i,k) * matrix_nc.at(k,j);

		current = tmp;
	}
};

}

#endif /* MATRIXSTACK_HPP_ */
