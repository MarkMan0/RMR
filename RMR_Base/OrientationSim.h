#pragma once
#pragma once
#include <chrono>

#include "Encoder.h"

class OrientationSim {
public:
	typedef typename Encoder<uint16_t, double> EncType;

	struct Position {
		double x, y, theta;
		double vx, vy, omega, v;
	};

private:
	EncType left, right;
	Encoder<signed short, double> theta;
	Position pos = { 0 };

	const double d, radius;

	using clock_t = EncType::clock_t;
	using now_t = decltype(clock_t::now());

	now_t lastTick;

public:
	OrientationSim(double _d, double _radius, double mmPerTick) : left(mmPerTick), right(mmPerTick), theta(1.0 / 100.0, -17999, 17999), d(_d), radius(_radius) { }

	void init(unsigned short l, unsigned short r, signed short theta, now_t now);
	void tick(uint16_t l, uint16_t r, signed short angle, now_t now);

	const EncType& getLeft() const {
		return left;
	}
	const EncType& getRight() const {
		return right;
	}

	double getTheta() const {
		return theta.getPosition();
	}

	Position getPosition() const {
		return pos;
	}

};
