#include "SCurve.h"
#include "SCurve.h"

#include <stdexcept>


scurve::SCurve scurve::SCurveGenerator::createCurve(Point from, Point to)
{
		SCurve curve(from, to);

	double distance = sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));

	double tAcc = maxVel / maxAcc;

	if (maxAcc * tAcc * tAcc > distance) {
		//no time to accelerate to maximum velocity
		tAcc = sqrt(distance / maxAcc);

		curve.addPoint(0, maxAcc);
		curve.addPoint(tAcc, -maxAcc);
		curve.addPoint(tAcc, 0, 0, distance);

	}
	else {
		double tVel = (distance - maxAcc * tAcc * tAcc) / maxVel;

		curve.addPoint(0, maxAcc);
		curve.addPoint(tAcc, 0, maxVel);
		curve.addPoint(tVel, -maxAcc);
		curve.addPoint(tAcc, 0, 0, distance);
	}

	return curve;
}

void scurve::SCurve::addPoint(double t, double val, std::optional<double> expVel, std::optional<double> expPos) {
	double lastT = 0;
	if (accPoints.empty()) {
		if (t != 0) {
			throw std::logic_error("First point needs to have 0 time");
		}
	}
	else {
		lastT = accPoints.back().t;
	}
	AccPoint p(lastT + t, val, expVel, expPos);
	accPoints.push_back(p);
}

scurve::SCurve::SCurve(Point _from, Point _to) : from(_from), to(_to) {
	direction = atan2(to.y - from.y, to.x - from.x);
}

bool scurve::SCurve::pointNow(Point& dest)
{
	if (!valid) {
		begin();
	}

	if (accPoints.size() <= 1) {
		accPoints.pop_front();	
		dest = to;
		return true;	//only the 0 left in queue
	}

	time_point now = clock::now();
	std::chrono::duration<double> sinceStart = now - startPoint;
	std::chrono::duration<double> dt = now - lastCall;
	lastCall = now;

	auto curr = accPoints.begin();
	auto next = std::next(curr);

	if (sinceStart.count() > ( next->t)) {
		//next point needed from list
		curr = next;
		accPoints.pop_front();
	}

	double accNow = curr->val;

	velocity += accNow * dt.count();
	if (curr->expectedVel) {
		velocity = *(curr->expectedVel);
	}
	double dPos = velocity * dt.count();
	if (curr->expectedPos) {
		dPos = *(curr->expectedPos) - position;
	}
	position += dPos;
	dest.x += dPos * cos(direction);
	dest.y += dPos * sin(direction);

	return false;

}

void scurve::SCurve::begin() {
	if (accPoints.empty()) {
		throw std::logic_error("No points in curve");
	}
	valid = true;
	startPoint = clock::now();
	lastCall = clock::now();
}
