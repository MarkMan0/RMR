#include "Orientation.h"
#include <iostream>
#include <limits>

void Orientation::init() {
	left.begin();
	right.begin();
	theta = 0;
	thetaZero = 0;
}


void Orientation::zeroHere() {
	left.zeroNow();
	right.zeroNow();
	thetaZero = theta;
	x = 0;
	y = 0;
}

void Orientation::tick(uint16_t l, uint16_t r, uint16_t dTheta) {
	left.tick(l);
	right.tick(r);
	int diff = 0;
	if (dTheta > std::numeric_limits<uint16_t>::max() - 10000) {
		diff = std::numeric_limits<uint16_t>::max() - dTheta;
		diff *= -1;
	}
	else {
		diff = dTheta;
	}
	double theta2 = theta + 1.0*diff;
	theta = theta2;
}