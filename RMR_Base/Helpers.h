#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

static inline double deg2rad(double deg) {
	return deg / 180.0 * M_PI;
}

static inline double rad2deg(double rad) {
	return rad / M_PI * 180.0;
}