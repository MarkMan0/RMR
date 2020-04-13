#include "MotionController.h"

#include <cmath>

#include "Helpers.h"
#include "RobotManager.h"
#include "LoopRate.h"
#include "ExitCondition.h"



bool MC::operator&(const MC::MovementType& a, const MC::MovementType& b) {
	return static_cast<bool>(static_cast<uint8_t>(a)& static_cast<uint8_t>(b));
}



void MC::MotionController::movementThread() {

	while (1) {
		if (!movements.empty()) {
			const auto target = movements.front();
			auto pos = robot->getPosition();

			if (target.type & MovementType::MOVEMENT_XY) {

				double pointAngleNow = rad2deg(atan2(target.y - pos.y, target.x - pos.x));
				double targetTheta = getClosestTargetAngle(pos.theta, pointAngleNow);
				rotationBlocking(targetTheta, 10);	//rotate towards target

				arcToXYBlocking(target.x, target.y);


			}
			if (target.type & MovementType::MOVEMENT_ROTATION) {
				rotationBlocking(target.theta);
			}
			movements.pop_front();
			robot->stop();
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
		.setKd(1)
		.setLower(-500)
		.setUpper(500)
		.setSampleT(1.0/50.0);

	angleController
		.setKp(0.1)
		.setKi(0.0)
		.setKd(0.05)
		.setLower(-1)
		.setUpper(1)
		.setSampleT(1.0/50.0);

	arcController
		.setKp(0.1)
		.setKi(0.0)
		.setKd(0.03)
		.setLower(-0.3)
		.setUpper(0.3)
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

	robot->stop();

}

void MC::MotionController::moveForward(double dist) {

	const auto pos = robot->getPosition();

	Movement mv;
	mv.type = MovementType::MOVEMENT_XY;
	mv.x = pos.x + dist * cos(deg2rad(pos.theta));
	mv.y = pos.y + dist * sin(deg2rad(pos.theta));

	movements.push_back(mv);
	cv.notify_all();
}

void MC::MotionController::rotateTo(double theta) {
	
	Movement mv;
	mv.type = MovementType::MOVEMENT_ROTATION;
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

	double spd = translationController.tick(eDist);
	double radius = spd / arcController.tick((eTheta));

	if (isnan(radius) || isinf(radius)) {
		radius = 0;
	}
	if (abs(eTheta) > 90) {
		spd *= -1;
		radius = 0;
	}
	robot->arc((int)round(spd), (int)round(radius));
}

void MC::MotionController::arcToXYBlocking(double x, double y) {

	LoopRate rate(50);
	ExitCondition cond(5, 100);
	auto pos = robot->getPosition();
	auto getDist = [&pos, &x, &y]()->double {
		return sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2));
		};

	double prevDist = getDist();
	
	while (!cond.check(getDist())) 	{
		arcControlTick(x, y);
		rate.sleep();
		pos = robot->getPosition();

		if (prevDist - getDist() < -50) {
			break;
		}
		prevDist = getDist();
	}

	robot->stop();


}

void MC::MotionController::arcToXY(double x, double y) {

	Movement mv;
	mv.type = MovementType::MOVEMENT_XY;
	mv.x = x;
	mv.y = y;

	movements.push_back(mv);
	cv.notify_all();
}