#include "Matrix.h"
#include "Point.h"
#include "Jointset.h"
#include <iostream>
#include <cmath>

#define PI 3.14159265

Matrix::Matrix(){
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			m[row][col] = 0;
		}
	}
}

Matrix::Matrix(double x[4][4]){
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			m[row][col] = x[row][col];
		}
	}
}

Matrix Matrix::multiply(const Matrix& other)const{
	Matrix result;
	for(int row=0; row<4; row++){
		for(int col=0; col<4; col++){
			for(int i=0; i<4; i++){
				result.m[row][col] += this->m[row][i] * other.m[i][col];
			}
		}
	}
	
	return result;
}

Matrix Matrix::transpose(){
	Matrix result;
	for(int row=0; row<3; row++){
		for(int col=0; col<3; col++){
			result.m[row][col] = this->m[col][row];
		}
	}
	
	return result;
}

Matrix Matrix::inverse(){
	Matrix result = this->transpose();
	result.m[0][3] = -(result.m[0][0]*this->m[0][3] + result.m[0][1]*this->m[1][3] + result.m[0][2]*this->m[2][3] + result.m[0][3]*this->m[3][3]);
	result.m[1][3] = -(result.m[1][0]*this->m[0][3] + result.m[1][1]*this->m[1][3] + result.m[1][2]*this->m[2][3] + result.m[1][3]*this->m[3][3]);
	result.m[2][3] = -(result.m[2][0]*this->m[0][3] + result.m[2][1]*this->m[1][3] + result.m[2][2]*this->m[2][3] + result.m[2][3]*this->m[3][3]);
	result.m[3][0] = 0; result.m[3][1] = 0; result.m[3][2] = 0; result.m[3][3] = 1;
	
	return result;
}

Point Matrix::transform(Point p){
	 double tmp[4][4] = {{p.x, 0, 0, 0},
				{p.y, 0, 0, 0},
				{p.z, 0, 0, 0},
				{1, 0, 0, 0}};
	Matrix temp(tmp);
	Matrix result = this->multiply(temp);
	Point p2(result.m[0][0], result.m[1][0], result.m[2][0]);
	
	return p2;
}

ostream& operator<<(ostream& out, const Matrix& matrix){
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			out << matrix.m[row][col] << " ";
		}
		out << endl;
	}
	return out;
}

const Matrix& Matrix::operator=(const Matrix& other){
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			m[row][col] = other.m[row][col];
		}
	}
	return *this;
}

Matrix Tforward(double d, double theta, double a, double alpha){
	double temp[4][4] = {{cos(theta * PI / 180.0),		-cos(alpha * PI / 180.0)*sin(theta * PI / 180.0),	sin(alpha * PI / 180.0)*sin(theta * PI / 180.0),	a*cos(theta * PI / 180.0)},
			     {sin(theta * PI / 180.0),		cos(alpha * PI / 180.0)*cos(theta * PI / 180.0),	-sin(alpha * PI / 180.0)*cos(theta * PI / 180.0),	a*sin(theta * PI / 180.0)},
			     {0.,						sin(alpha * PI / 180.0),				cos(alpha * PI / 180.0),			d		 },
			     {0.,							0.,							0.,					1.		}};
			     
	return Matrix(temp);
}

Matrix operator*(const Matrix& c1, const Matrix& c2){
	return c1.multiply(c2);
}

Matrix T50(double th1, double th2, double th3, double th4, double th5){
	 double d[5] = {27.2, 0., 0., 0., 10.5};
	 double a[5] = {0., 19.2, 19.2, 0., 0.};
	 double alpha[5] = {90., 180., 0., 90., 0.};
	
	Matrix transforms[5];
	transforms[0] = Tforward(d[0], th1, a[0], alpha[0]);
	transforms[1] = Tforward(d[1], th2, a[1], alpha[1]);
	transforms[2] = Tforward(d[2], th3, a[2], alpha[2]);
	transforms[3] = Tforward(d[3], th4 + 90.0, a[3], alpha[3]);
	transforms[4] = Tforward(d[4], th5, a[4], alpha[4]);
	
	Matrix result = transforms[0]*transforms[1]*transforms[2]*transforms[3]*transforms[4];
	
	return result;
}

Matrix A03(double& th1, double& th2, double& th3){
	 double temp[4][4] = {{cos(th1*PI/180.0)*cos((th3-th2)*PI/180.0),    sin(th1*PI/180.0)*cos((th3-th2)*PI/180.0),  -sin((th3-th2)*PI/180.0),  -19.2-19.2*cos(th3*PI/180.0)+27.2*sin((th3-th2)*PI/180.0)},
			     {-cos(th1*PI/180.0)*sin((th3-th2)*PI/180.0),  -sin(th1*PI/180.0)*sin((th3-th2)*PI/180.0),  -cos((th3-th2)*PI/180.0),  19.2*sin(th3*PI/180.0)+27.2*cos((th3-th2)*PI/180.0)},
			     {-sin(th1*PI/180.0),				cos(th1*PI/180.0),			0,				0},
			     {0,						0,					0,				1}};
			     
	return Matrix(temp);
}

Jointset inverseKin(const Matrix& x){
	Jointset result;
	
	double translation[3] = {x.m[0][3] - 10.5 * x.m[0][2],
				 x.m[1][3] - 10.5 * x.m[1][2],
				 x.m[2][3] - 10.5 * x.m[2][2]};
//	cout << "translations" << endl << translation[0] << endl << translation[1] << endl << translation[2] << endl;
				 
	// Ternary may break stuff												//			y,	x
	result.angles[0] = atan2(abs(translation[1])<1e-07?0:translation[1], translation[0]) / PI * 180.0;					// theta 1 = atan2(ty-d5ay, tx-d5ax)
	
	double a2 = translation[0]*translation[0] + translation[1]*translation[1];		// a^2 = x^2 + y^2
	double a = sqrt(a2);
	double z = translation[2] - 27.2;
	double z2 = z*z;

	result.angles[2] = abs(acos((a2 + z2 - 19.2*19.2 - 19.2*19.2)/(2*19.2*19.2)>1?1:(a2 + z2 - 19.2*19.2 - 19.2*19.2)/(2*19.2*19.2)) / PI * 180.0);		// theta 3
	
	result.angles[1] = atan2(z, a) / PI * 180.0 + atan2(19.2*sin(result.angles[2] * PI / 180.0), 19.2 + 19.2 * cos(result.angles[2] * PI / 180.0)) / PI * 180.0;	// theta 2
	
	Matrix temp = A03(result.angles[0], result.angles[1], result.angles[2]);
	Matrix A53 = temp * x;
	
	result.angles[3] = atan2(A53.m[1][2], A53.m[0][2]) / PI * 180.0;			// theta 4
	result.angles[4] = atan2(A53.m[2][0], A53.m[2][1]) / PI * 180.0;			// theta 5

	return result;
}

