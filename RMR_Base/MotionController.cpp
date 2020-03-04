#include "MotionController.h"

#include <cmath>

#include "Helpers.h"
#include "RobotManager.h"
#include "LoopRate.h"
#include "ExitCondition.h"


void MotionController::init() {
	translationController
		.setKp(2)
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

	arcController
		.setKp(0.08)
		.setKi(0.0)
		.setKd(0.0)
		.setLower(-0.2)
		.setUpper(0.2)
		.setSampleT(1.0 / 50.0);

}

bool MotionController::rotateTo(double target, double tolerance)
{
	angleController.enable();
	bool done = false;

	ExitCondition cond(25, tolerance);

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
			robot->translation((int)round(translationController.tick(e)));
			rate.sleep();
		}
	}

	robot->translation(0);

	return true;
}

bool MotionController::moveArc(double x, double y) {
	translationController.enable();
	angleController.enable();
	arcController.enable();
	const auto posStart = robot->getPosition();
	auto posNow = robot->getPosition();

	auto distToTarget = [&posNow, x, y]() -> double {
		return sqrt(pow(x - posNow.x, 2) + pow(y - posNow.y, 2));
		};

	LoopRate rate(50);
	ExitCondition cond(40, 30);

	while (!cond.check(distToTarget())) {
		posNow = robot->getPosition();
		double eDist = distToTarget();		//absolute distance to target
		
		double pointAngleNow = rad2deg(atan2(y - posNow.y, x - posNow.x));
		double targetTheta = getClosestTargetAngle(posNow.theta, pointAngleNow);
		
		double eTheta = targetTheta - posNow.theta;

		if (!cond.check(eDist)) {
			double v = translationController.tick(eDist);
			double u = v/angleController.tick((eTheta));
			robot->arc((int)round(v), u);
		}

		rate.sleep();
	}

	robot->stop();
	

	return true;
}

bool MotionController::arcToXY(double x, double y) {
	
	auto pos = robot->getPosition();
	
	double pointAngle = rad2deg(atan2(y - pos.y, x - pos.x));
	double target = getClosestTargetAngle(pos.theta, pointAngle);

	rotateTo(target, 90);	//rotate towards target, +-x degrees
	moveArc(x, y);


	return true;
}