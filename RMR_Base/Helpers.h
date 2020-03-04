#pragma once
#include <cmath>

static inline double deg2rad(double deg) {
	return deg / 180.0 * M_PI;
}

static inline double rad2deg(double rad) {
	return rad / M_PI * 180.0;
}

template<class T>
static inline T clamp(T what, T bot, T top) {
	return what <= bot ? bot : what >= top ? top : what;
}


template<class T>
static inline double scaleAngle(T angle) {
	angle = fmod(angle, 360.0f);
	if (angle > 180)
	{
		angle -= 360;
	}
	if (angle < -180)
	{
		angle += 360;
	}
	return angle;
}


static inline double getClosestTargetAngle(double actual, double target) {
	//return the closes point to target
	//Example: robot is 90, target is 350, the result will be -10
	//Example2: robot is 180, target is 190, result: 190

	double scaled = scaleAngle(actual);	//robot absolute position in (-180, 180]
	double point = scaleAngle(target); //target angle scaled

	double diff = point - scaled;

	return actual + diff;

}