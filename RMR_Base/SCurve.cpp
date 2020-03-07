#include "SCurve.h"
#include "SCurve.h"

#include <stdexcept>


SCurve::SCurve SCurve::SCurveGenerator::createCurve(Point from, Point to)
{
	using namespace SCurve;

	SCurve curve(from, to);

	double distance = sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));

	double tAcc = maxVel / maxAcc;

	if (maxAcc * tAcc * tAcc > distance) {
		//no time to accelerate to maximum velocity
		tAcc = sqrt(distance / maxAcc);

		curve.addPoint(0, maxAcc);
		curve.addPoint(tAcc, -maxAcc);
		curve.addPoint(tAcc, 0, 0);

	}
	else {
		double tVel = (distance - maxAcc * tAcc * tAcc) / maxVel;

		curve.addPoint(0, maxAcc);
		curve.addPoint(tAcc, 0, maxVel);
		curve.addPoint(tVel, -maxAcc);
		curve.addPoint(tAcc, 0, 0);
	}

	return curve;
}

void SCurve::SCurve::addPoint(double t, double val, double expected /* =-1 */) {
	double lastT = 0;
	if (accPoints.empty()) {
		if (t == 0) {
			throw std::logic_error("First point needs to have 0 time");
		}
	}
	else {
		lastT = accPoints.back().t;
	}

	accPoints.push_back({ lastT + t, val, expected });
}

SCurve::SCurve::SCurve(Point _from, Point _to) : from(_from), to(_to) {
	direction = atan2(to.y - from.y, to.x - from.x);
}

bool SCurve::SCurve::pointNow(Point& dest)
{
	using namespace SCurve;
	if (!valid) {
		throw std::logic_error("SCurve wasn't started(begin())");
	}

	if (accPoints.size() <= 1) {
		accPoints.pop_front();	
		dest = to;
		return true;	//only the 0 left in queue
	}

	time_point now = clock::now();
	std::chrono::duration<double> sinceStart = now - startPoint;
	std::chrono::duration<double> dt = now - lastCall;

	auto curr = accPoints.begin();
	auto next = std::next(curr);

	if (sinceStart.count() > (sinceStart.count() + next->t)) {
		//next point needed from list
		curr = next;
		accPoints.pop_front();
	}

	double accNow = curr->val;

	velocity += accNow * dt.count();
	if (curr->expectedVel != -1) {
		velocity = curr->expectedVel;
	}
	position += velocity * dt.count();
	
	dest.x = position * cos(direction);
	dest.y = position * sin(direction);

	return false;

}

void SCurve::SCurve::begin() {
	if (accPoints.empty()) {
		throw std::logic_error("No points in curve");
	}
	valid = true;
	startPoint = clock::now();
	lastCall = clock::now();
}
