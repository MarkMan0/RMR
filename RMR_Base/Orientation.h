#pragma once
#include "Encoder.h"

class Orientation {
public:
	typedef typename Encoder<uint16_t, double> EncType;

	struct Position {
		double x, y, theta;
	};

private:
	EncType left, right;
	Encoder<signed short, double> theta;
	double x = 0, y = 0;

	const double d, radius;


public:
	Orientation(double _d, double _radius, double mmPerTick) : left(mmPerTick), right(mmPerTick), theta(1.0/100.0), d(_d), radius(_radius) { }

	void init(unsigned short l = 0, unsigned short r = 0, signed short theta = 0);	
	void tick(uint16_t l, uint16_t r, signed short angle);

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
		Position p = { x, y, theta.getPosition() };
		return p;
	}

};
