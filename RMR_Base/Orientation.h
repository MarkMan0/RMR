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
	Orientation(double _d, double _radius, double tick2mm) : left(tick2mm), right(tick2mm), theta(1.0/100.0), d(_d), radius(_radius) { }

	void init();
	void zeroHere();	
	void tick(uint16_t l, uint16_t r, signed short angle);

	const EncType& getLeft() const {
		return left;
	}
	const EncType& getRight() const {
		return right;
	}
	
	double getTheta() const {
		return theta.getPosition()/100;
	}

	Position getPosition() const {
		Position p = { x, y, theta.getPosition() };
		return p;
	}

};
