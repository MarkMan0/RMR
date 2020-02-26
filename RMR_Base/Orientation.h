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
	double x = 0, y = 0;
	double theta = 0, thetaLast = 0;

	const double d, radius;


public:
	Orientation(double _d, double _radius, double tick2mm) : left(tick2mm), right(tick2mm), d(_d), radius(_radius) { }

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
		return theta/100;
	}

	Position getPosition() const {
		Position p = { x, y, theta };
		return p;
	}

};
