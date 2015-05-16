#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "Jointset.h"
#include "Point.h"

using namespace std;

class Matrix{
public:
	double m[4][4];
	Matrix();
	Matrix(double x[4][4]);
	Matrix multiply(const Matrix& other) const;
	Matrix transpose();
	Matrix inverse();
	Point transform(Point p);
	
	friend ostream& operator<<(ostream& out, const Matrix& matrix);
//	friend istream& operator>>(istream& in, Matrix& matrix);
	
	const Matrix& operator=(const Matrix& other);
};

Matrix Tforward(double d, double theta, double a, double alpha);
Matrix T50(double th1, double th2, double th3, double th4, double th5);

Matrix A03(double& th1, double& th2, double& th3);
Jointset inverseKin(const Matrix& x);

Matrix operator*(const Matrix& c1, const Matrix& c2);

#endif
