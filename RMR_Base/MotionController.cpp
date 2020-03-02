#include "MotionController.h"

#include <cmath>


#include "RobotManager.h"
#include "LoopRate.h"
#include "ExitCondition.h"

static inline double deg2rad(double deg) {
	return deg / 180.0 * M_PI;
}

void MotionController::init() {
	translationController
		.setKp(5)
		.setKi(0.0)
		.setKd(0.0)
		.setLower(-100)
		.setUpper(100)
		.setSampleT(1.0/50.0);

	angleController
		.setKp(0.2)
		.setKi(0.0)
		.setKd(0.0)
		.setLower(-1)
		.setUpper(1)
		.setSampleT(1.0/50.0);

}

bool MotionController::rotateTo(double target)
{
	angleController.enable();
	bool done = false;

	ExitCondition cond(25, 0.5);

	LoopRate rate(50);
	while (!done) {
		auto pos = robot->getPosition();
		double e = target - pos.theta;

		if (cond.check(e)) {
			done = true;
		}
		else {
			double u = angleController.tick(e);
			robot->rotation(u);
			rate.sleep();
		}
	}

	robot->rotation(0);

	return true;
}

bool MotionController::moveForward(double dist) {

	const auto oldPos = robot->getPosition();
	auto pos = robot->getPosition();

	auto getDistMoved = [&pos, &oldPos]()->double {return sqrt(pow(pos.x - oldPos.x, 2) + pow(pos.y - oldPos.y, 2)); };

	auto getSpd = [&pos]()->double {return sqrt(pow(pos.vx, 2) + pow(pos.vy, 2)); };
	translationController.enable();
	
	LoopRate rate(50);
	ExitCondition cond(25, 10);
	bool done = false;

	while (!done) {
		pos = robot->getPosition();
		double e = dist - getDistMoved();
	
		if (cond.check(e)) {
			done = true;
		}
		else {
			robot->translation(translationController.tick(e));
			rate.sleep();
		}
	}

	robot->translation(0);

	return true;
}

