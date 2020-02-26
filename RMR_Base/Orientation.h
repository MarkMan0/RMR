#pragma once
#include "Encoder.h"

class Orientation {
public:
	typedef typename Encoder<uint16_t, double> EncType;

private:
	EncType left, right;
	double x = 0, y = 0;
	double theta = 0, thetaLast = 0, thetaZero = 0;

	const double d, radius;


public:
	Orientation(double _d, double _radius) : d(_d), radius(_radius) { }

	void init();
	void zeroHere();	
	void tick(uint16_t l, uint16_t r, signed short angle);
	
	double getTheta() const {
		return theta/100;
	}

};
