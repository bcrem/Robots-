#include "Jointset.h"

Jointset::Jointset(){
	for(int i = 0; i < 5; i++){
		angles[i] = 0;
	}
}

Jointset::Jointset(double a, double b, double c, double d, double e){
	angles[0] = a;
	angles[1] = b;
	angles[2] = c;
	angles[3] = d;
	angles[4] = e;
}

const Jointset& Jointset::operator=(const Jointset& other){
	for(int i = 0; i < 5; i++){
		angles[i] = other.angles[i];
	}
	
	return *this;
}

Jointset operator-(const Jointset& a, const Jointset& b){
	double t1 = a.angles[0] - b.angles[0];
	double t2 = a.angles[1] - b.angles[1];
	double t3 = a.angles[2] - b.angles[2];
	double t4 = a.angles[3] - b.angles[3];
	double t5 = a.angles[4] - b.angles[4];
	
	return Jointset(t1, t2, t3, t4, t5);
}
