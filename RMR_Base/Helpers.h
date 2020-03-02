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