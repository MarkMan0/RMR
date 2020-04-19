#pragma once
#include <cmath>

constexpr double deg2rad(double deg) {
	return deg / 180.0 * M_PI;
}

constexpr double rad2deg(double rad) {
	return rad / M_PI * 180.0;
}

template<class T>
constexpr T clamp(T what, T bot, T top) {
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

	double diff = scaleAngle(point - scaled);

	return actual + diff;

}


template<class T>
constexpr int sign(T num) {
	if (num < 0) return -1;
	if (num > 0) return 1;
	return 0;
}


static inline double angleDiff(double a, double b) {
	a = scaleAngle(a);
	b = scaleAngle(b);
	if (a < 0) a += 360;
	if (b < 0) b += 360;

	return abs(a - b);
}

template<class T = double>
struct BasePoint {
	T x;
	T y;
	bool operator <(const BasePoint<T>& rhs) const {
		if (this->x != rhs.x) 
			return this->x < rhs.x;
		else 
			return this->y < rhs.y;
	}

	bool operator==(const BasePoint<T>& rhs) const {
		return (this->x == rhs.x && this->y == rhs.y);
	}

	template <class U>
	operator BasePoint<U>()
	{
		BasePoint<U> u;
		u.x = static_cast<U>(x);
		u.y = static_cast<U>(y);
		return u;
	}

	BasePoint<T>() = default;
	BasePoint<T>(T _x, T _y) : x(_x), y(_y) {}

	double dirTo(const BasePoint<T>& other) const {
		return atan2(other.y - y, other.x - x);
	}
	double dist(const BasePoint<T>& other) const {
		return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
	}

};
using Point = BasePoint<double>;