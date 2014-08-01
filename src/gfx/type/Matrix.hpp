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

	const T* data() const {
		return _data.data();
	}



	static Matrix<4, 4, T> Identity() {
		Matrix<4, 4, T> mat;
		for(int i=0; i<4; ++i) mat.at(i,i) = 1;
		return mat;
	}

	static Matrix<4, 4, T> Translation(T dx, T dy, T dz) {
		Matrix<4, 4, T> mat = Identity();
		mat.at(0,3) = dx;
		mat.at(1,3) = dy;
		mat.at(2,3) = dz;
		return mat;
	}

	static Matrix<4, 4, T> Scale(T sx, T sy, T sz) {
		Matrix<4, 4, T> mat = Identity();
		mat.at(0,0) = sx;
		mat.at(1,1) = sy;
		mat.at(2,2) = sz;
		return mat;
	}

	static Matrix<4, 4, T> RotationX(T degAngle) {
		static T degToRad = 3.14/180;
		T vSin = sin(degAngle * degToRad);
		T vCos = cos(degAngle * degToRad);
		return {
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	vCos,	-vSin,	0.0f,
			0.0f,	vSin,	vCos,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
	}

	static Matrix<4, 4, T> RotationY(T degAngle) {
		static T degToRad = 3.14/180;
		T vSin = sin(degAngle * degToRad);
		T vCos = cos(degAngle * degToRad);
		return {
			vCos,	0.0f,	vSin,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			-vSin,	0.0f,	vCos,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
	}

	static Matrix<4, 4, T> RotationZ(T degAngle) {
		static T degToRad = 3.14/180;
		T vSin = sin(degAngle * degToRad);
		T vCos = cos(degAngle * degToRad);
		return {
			vCos,	-vSin,	0.0f,	0.0f,
			vSin,	vCos,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f
		};
	}
};

}

#endif /* MATRIX_HPP_ */
