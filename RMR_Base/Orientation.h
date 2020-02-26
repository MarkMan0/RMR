#pragma once
#include "Encoder.h"

class Orientation {
public:
	typedef typename Encoder<uint16_t, double> EncType;

private:
	EncType left, right;
	double theta = 0, x = 0, y = 0, thetaZero = 0;

	double d, radiusL, radiusR;


public:
	void init();

	void zeroHere();	

	void tick(uint16_t l, uint16_t r, uint16_t dTheta);
	
	double getTheta() const {
		return theta/100;
	}

};
