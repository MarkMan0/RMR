#define _USE_MATH_DEFINES
#include <math.h>
#include "MotionController.h"
#include "RobotManager.h"

#include "LoopRate.h"

static inline double deg2rad(double deg) {
	return deg / 180.0 * M_PI;
}

void MotionController::init() {
	translationController
		.setKp(0.0)
		.setKi(0.0)
		.setKd(0.0)
		.setLower(0.0)
		.setUpper(0.0)
		.setSampleT(0.0);

	angleController
		.setKp(0.0)
		.setKi(0.0)
		.setKd(0.0)
		.setLower(0.0)
		.setUpper(0.0)
		.setSampleT(0.0);

}

bool MotionController::rotateTo(double target)
{
	angleController.enable();
	bool done = false;
	LoopRate rate(50);
	while (!done) {
		auto pos = robot->getPosition();
		if (abs(target - pos.theta) < 2 && pos.omega < 10) {
			//close to target and slow
			done = true;
		}
		else {
			double u = angleController.tick(target - pos.theta);
			robot->rotation(u);
		}
		rate.sleep();
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

	while (abs(dist - getDistMoved()) < 10 && getSpd() < 10) {

		double e = dist - getDistMoved();
		
		robot->translation(translationController.tick(e));

		rate.sleep();
		pos = robot->getPosition();
	}

	robot->translation(0);

	return true;
}

