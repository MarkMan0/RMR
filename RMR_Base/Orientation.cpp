#define _USE_MATH_DEFINES
#include "Orientation.h"
#include <iostream>
#include <limits>
#include <math.h>

void Orientation::init(unsigned short l, unsigned short r, signed short theta) {
	left.begin(l);
	right.begin(r);
	this->theta.begin(theta);

}


void Orientation::tick(uint16_t l, uint16_t r, signed short angle) {
	double dl = left.tick(l);
	double dr = right.tick(r);

	double thetaLast = theta.getPosition();
	theta.tick(angle);
	double thetaNow = theta.getPosition();
	

	if ( abs(dr-dl) > 1e-5) {
		x += d * (dr + dl) / (2.0 * (dr - dl)) * ( sin(thetaNow/360.0*2*M_PI) - sin(thetaLast/360.0*2*M_PI) );
		y -= d * (dr + dl) / (2.0 * (dr - dl)) * ( cos(thetaNow/360.0*2*M_PI) - cos(thetaLast/360.0*2*M_PI) );
	}
	else {
		x += dl * cos(thetaNow / 360.0 * 2 * M_PI);
		y += dr * sin(thetaNow / 360.0 * 2 * M_PI);
	}
	std::cout << "x: \t" << x << "\ty: \t" << y << "\ttheta:\t" << thetaNow << std::endl;

}