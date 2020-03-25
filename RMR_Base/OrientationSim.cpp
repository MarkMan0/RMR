#include "OrientationSim.h"

#include <iostream>
#include <cmath>

#include "Helpers.h"

void OrientationSim::init(unsigned short l, unsigned short r, signed short theta, now_t now) {
	left.begin(l, now);
	right.begin(r, now);
	this->theta.begin(theta, now);

	pos.x = 0;
	pos.y = 0;
	pos.theta = this->theta.getPosition();

	pos.vx = 0;
	pos.vy = 0;
	pos.omega = 0;
	pos.v = 0;

}


void OrientationSim::tick(uint16_t l, uint16_t r, signed short angle, now_t now) {
	double dl = left.tick(l, now);
	double dr = right.tick(r, now);

	double thetaLast = deg2rad(theta.getPosition());
	theta.tick(angle, now);
	double thetaNow = deg2rad(theta.getPosition());

	double oldX = pos.x, oldY = pos.y;

	if (abs(dr - dl) > 1e-1 && abs(thetaNow - thetaLast) > deg2rad(0.5)) {
		pos.x += d * (dr + dl) / (2.0 * (dr - dl)) * (sin(thetaNow) - sin(thetaLast));
		pos.y -= d * (dr + dl) / (2.0 * (dr - dl)) * (cos(thetaNow) - cos(thetaLast));
		//std::cout << "XXX: ";
	}
	else {
		double l = (dl + dr) / 2;
		pos.x += l * cos(thetaNow);
		pos.y += l * sin(thetaNow);
		//std::cout << "OOO: ";
	}

	std::chrono::duration<double> dt = now - lastTick;
	lastTick = now;

	pos.vx = (pos.x - oldX) / dt.count();
	pos.vy = (pos.y - oldY) / dt.count();
	pos.omega = theta.getSpeed();
	pos.theta = theta.getPosition();
	pos.v = sqrt(pow(pos.vx, 2) + pow(pos.vy, 2));
}