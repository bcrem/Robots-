#include "Point.h"
#include <iostream>

Point::Point(){
	x = 0;
	y = 0;
	z = 0;
}

Point::Point(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}

const Point& Point::operator=(const Point& other){
	x = other.x;
	y = other.y;
	z = other.z;
	
	return *this;
}

ostream& operator<<(ostream& out, const Point& point){
	out << point.x << " " << point.y << " " << point.z << endl;
	return out;
}

Point operator-(const Point& a, const Point& b){
	return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator+(const Point& a, const Point& b){
	return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator*(const Point& a, const int b){
	return Point(a.x * b, a.y * b, a.z * b);
}
