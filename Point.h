#ifndef POINT_H
#define POINT_H

#include <iostream>

using namespace std;

class Point{
public:
	double x,y,z;
	Point();
	Point(double x, double y, double z);
	const Point& operator=(const Point& other);
	friend ostream& operator<<(ostream& out, const Point& point);
};
Point operator-(const Point& a, const Point& b);
Point operator+(const Point& a, const Point& b);
Point operator*(const Point& a, const int b);

#endif
