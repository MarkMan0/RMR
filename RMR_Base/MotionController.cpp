#include "MotionController.h"

#include <cmath>

#include "Helpers.h"
#include "RobotManager.h"
#include "LoopRate.h"
#include "ExitCondition.h"


void MC::MotionController::movementThread() {

	while (1) {
		if (!movements.empty()) {
			const auto target = movements.front();
			auto pos = robot->getPosition();

			if (target.type & MOVEMENT_XY) {
				auto curve = sGenerator.createCurve({ pos.x, pos.y }, { target.x, target.y });
				SCurve::Point p(0, 0);
				LoopRate rate(50);

				double theta = atan2(target.y - pos.y, target.x - pos.x);
				rotationBlocking(theta, 20);	//rotate towards target

				//move the rest of the way on an s-curve
				while (!curve.pointNow(p)) {
					arcControlTick(p.x, p.y);
					rate.sleep();
				}
			}
			if (target.type & MOVEMENT_ROTATION) {
				rotationBlocking(target.theta);
			}
			movements.pop_front();
		}

		if (movements.empty()) {
			std::unique_lock lck(cvMtx);
			cv.wait(lck, [this]()->bool {return !(movements.empty()); });
		}
	}
}

void MC::MotionController::init() {
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

	if (!plannerThread.joinable()) {
		plannerThread = std::thread(&MotionController::movementThread, this);
	}
}

void MC::MotionController::rotationBlocking(double target, double tolerance)
{
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

}

void MC::MotionController::moveForward(double dist) {

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

}

void MC::MotionController::rotateTo(double theta) {
	
	Movement mv;
	mv.type = MOVEMENT_ROTATION;
	mv.theta = theta;

	movements.push_back(mv);
	cv.notify_all();

}

void MC::MotionController::arcControlTick(double x, double y) {


	const auto posNow = robot->getPosition();
	double eDist = sqrt(pow(x - posNow.x, 2) + pow(y - posNow.y, 2));

	double pointAngleNow = rad2deg(atan2(y - posNow.y, x - posNow.x));
	double targetTheta = getClosestTargetAngle(posNow.theta, pointAngleNow);
	double eTheta = targetTheta - posNow.theta;

	double v = translationController.tick(eDist);
	double u = v / angleController.tick((eTheta));
	robot->arc((int)round(v), (int)round(v));
}

void MC::MotionController::arcToXY(double x, double y) {

	Movement mv;
	mv.type = MOVEMENT_XY;
	mv.x = x;
	mv.y = y;

	movements.push_back(mv);
	cv.notify_all();
}