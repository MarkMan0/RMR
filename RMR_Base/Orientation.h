#pragma once
#include <chrono>
#include <iostream>
#include "Config.h"
#include "Encoder.h"
#include "Helpers.h"

class Orientation {
public:
	typedef typename Encoder<uint16_t, double> EncType;

	struct Position {
		Point p;
		double& x, & y;
		double theta = 0;
		double vx = 0, vy = 0, omega = 0, v = 0;
		void print() {
			std::cout << "x:\t" << x << "\t\ty:\t" << y << "\t\tTheta:\t" << theta << "\n";
		}
		Position() : p(0, 0), x(p.x), y(p.y) {}

		Position& operator=(const Position& rhs) {
			p = rhs.p;
			theta = rhs.theta;
			vx = rhs.vx;
			vy = rhs.vy;
			omega = rhs.omega;
			v = rhs.v;
			return *this;
		}
		Position(const Position& rhs) : p(rhs.p), x(p.x), y(p.y), theta(rhs.theta), vx(rhs.vx), vy(rhs.vy), omega(rhs.omega), v(rhs.v) {}
	};

private:
	EncType left, right;
	Encoder<signed short, double> theta;
	Position pos;

	const double d, radius;

	using clock = std::chrono::steady_clock;

	decltype(clock::now()) lastTick;

public:
	Orientation() : left(config::rob::tick2mm), right(config::rob::tick2mm), 
		theta(config::rob::tick2deg, config::rob::thetaMin, config::rob::thetaMax),
		d(config::rob::d), radius(config::rob::r) { }

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
		return pos;
	}

};
