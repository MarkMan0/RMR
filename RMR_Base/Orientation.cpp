
#include "Orientation.h"

#include <iostream>
#include <cmath>

#include "Helpers.h"

void Orientation::init(unsigned short l, unsigned short r, signed short theta) {
	left.begin(l);
	right.begin(r);
	this->theta.begin(theta);
	x = 0;
	y = 0;

}


void Orientation::tick(uint16_t l, uint16_t r, signed short angle) {
	double dl = left.tick(l);
	double dr = right.tick(r);

	double thetaLast = deg2rad(theta.getPosition());
	theta.tick(angle);
	double thetaNow = deg2rad(theta.getPosition());
	

	if ( abs(dr-dl) > 1e-1 && abs(thetaNow - thetaLast) > deg2rad(0.5)) {
		x += d * (dr + dl) / (2.0 * (dr - dl)) * ( sin(thetaNow) - sin(thetaLast) );
		y -= d * (dr + dl) / (2.0 * (dr - dl)) * ( cos(thetaNow) - cos(thetaLast) );
		//std::cout << "XXX: ";
	}
	else {
		double l = (dl + dr) / 2;
		x += l * cos(thetaNow);
		y += l * sin(thetaNow);
		//std::cout << "OOO: ";
	}

	

}