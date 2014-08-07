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
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace gfx {

class MatrixStack {

private:
	stack<glm::mat4> matrices;
	glm::mat4 current = glm::mat4(1.0f);

public:

	const glm::mat4& top() {
		return current;
	}

	void set(const glm::mat4& top) {
		current = top;
	}

	void apply(const glm::mat4& mat) {
		current *= mat;
	}

	void push() {
		matrices.push(current);
	}

	void pop() {
		current = matrices.top();
		matrices.pop();
	}

	// used by 'with'
	void bind() {
		push();
	}

	// used by 'with'
	void unbind() {
		pop();
	}

	void translate(const glm::vec3& v) {
		current = glm::translate(current, v);
	}

	template <typename... V>
	void translate(V... v) {
		translate(glm::vec3(v...));
	}

	void scale(const glm::vec3& v) {
		current = glm::scale(current, v);
	}

	template <typename... V>
	void scale(V... v) {
		scale(glm::vec3(v...));
	}

	void rotateX(float v) {
		current = glm::rotate(current, v, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	void rotateY(float v) {
		current = glm::rotate(current, v, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void rotateZ(float v) {
		current = glm::rotate(current, v, glm::vec3(0.0f, 0.0f, 1.0f));
	}
};

//
//template <unsigned int rows, unsigned int cols, typename T>
//class MatrixStack {
//
//	typedef Matrix<rows, cols, T> TMatrix;
//
//private:
//
//	stack<TMatrix> matrices;
//	TMatrix current = TMatrix::Identity();
//
//public:
//
//	const TMatrix& top() {
//		return current;
//	}
//
//	void push() {
//		matrices.push(current);
//	}
//
//	void pop() {
//		current = matrices.top();
//		matrices.pop();
//	}
//
//	void mul(const TMatrix& matrix) {
//		if (rows != cols) throw logic_error("non-square matrix multiplication");
//		TMatrix tmp;
//
//		TMatrix& matrix_nc = const_cast<TMatrix&>(matrix);
//
//		for (int i=0; i<rows; i++)
//			for (int j=0; j<cols; j++)
//				for (int k=0; k <rows; ++k)
//					tmp.at(i,j) += current.at(i,k) * matrix_nc.at(k,j);
//
//		current = tmp;
//	}
//};

}

#endif /* MATRIXSTACK_HPP_ */
