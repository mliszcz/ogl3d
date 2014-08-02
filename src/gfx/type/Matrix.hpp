/*
 * Matrix.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: michal
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include "../../Common.hpp"

//#include "Vector.hpp"

namespace gfx {

// have no fuckin idea why this works ...
// Vector is incomplete-type here (??)
template<unsigned int dim, typename T>
class Vector;

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

	static Matrix<4, 4, T> Translation(const Vector<3, T>& v) {
		return Translation(v.data()[0], v.data()[1], v.data()[2]);
	}

	static Matrix<4, 4, T> Translation(T t1, T t2, T t3) {
		Matrix<4, 4, T> mat = Identity();
		mat.at(0,3) = t1;
		mat.at(1,3) = t2;
		mat.at(2,3) = t3;
		return mat;
	}

	static Matrix<4, 4, T> Scale(const Vector<3, T>& v) {
		return Scale(v.data()[0], v.data()[1], v.data()[2]);
	}

	static Matrix<4, 4, T> Scale(T s1, T s2, T s3) {
		Matrix<4, 4, T> mat = Identity();
		mat.at(0,0) = s1;
		mat.at(1,1) = s2;
		mat.at(2,2) = s3;
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
