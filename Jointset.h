#ifndef JOINTSET_H
#define JOINTSET_H

using namespace std;

class Jointset{
public:
	double angles[5];
	Jointset();
	Jointset(double a, double b, double c, double d, double e);
	
	const Jointset& operator=(const Jointset& other);
};

Jointset operator-(const Jointset& a, const Jointset& b);

#endif
