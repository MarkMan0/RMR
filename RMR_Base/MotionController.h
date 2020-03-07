#pragma once
#include "PIDController.h"
#include "RobotManager.h"

#include <thread>
#include <memory>

class MotionController {
private:
	PIDController translationController;
	PIDController angleController;
	PIDController arcController;

	std::shared_ptr<RobotManager> robot;

	std::thread plannerThread;

public:
	MotionController(const std::shared_ptr<RobotManager>& _manager) : robot(_manager) {}

	void init();

	bool rotateTo(double target, double tolerance = 0.5);

	bool moveForward(double dist);
	
	bool arcToXY(double x, double y);

	bool moveArc(double x, double y);
};

