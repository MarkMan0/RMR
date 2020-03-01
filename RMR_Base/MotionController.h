#pragma once
#include "PIDController.h"
#include "RobotManager.h"

#include <memory>

class MotionController {
private:
	PIDController translationController;
	PIDController angleController;

	std::shared_ptr<RobotManager> robot;

public:
	MotionController(const std::shared_ptr<RobotManager>& _manager) : robot(_manager) {}

	void init();

	bool rotateTo(double target);

	bool moveForward(double dist);
	
	bool moveToXY(double x, double y);
};

