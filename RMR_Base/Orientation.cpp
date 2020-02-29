#define _USE_MATH_DEFINES
#include "Orientation.h"
#include <iostream>
#include <limits>
#include <math.h>

void Orientation::init() {
	left.begin(0);
	right.begin(0);
	theta.begin(0);

}


void Orientation::zeroHere() {
	x = 0;
	y = 0;
}

void Orientation::tick(uint16_t l, uint16_t r, signed short angle) {
	double dl = left.tick(l);
	double dr = right.tick(r);

	double thetaLast = theta.getPosition();
	theta.tick(angle);
	double thetaNow = theta.getPosition();
	

	if (abs(dl - dr) > 1) {
		x += d * (dr + dl) / (2.0 * (dr - dl)) * ( sin(thetaNow /100.0/360.0*2*M_PI) - sin(thetaLast /100.0/360.0*2*M_PI) );
		y -= d * (dr + dl) / (2.0 * (dr - dl)) * ( cos(thetaNow /100.0/360.0*2*M_PI) - cos(thetaLast /100.0/360.0*2*M_PI) );
	}
	else {
		x += dl * cos(thetaNow / 100.0 / 360.0 * 2 * M_PI);
		y += dr * sin(thetaNow / 100.0 / 360.0 * 2 * M_PI);
	}
	std::cout << "x: \t" << x << "\ty: \t" << y << "\ttheta:\t" << thetaNow /100.0 << std::endl;

}