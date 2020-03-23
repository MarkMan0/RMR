#pragma once
#include "PIDController.h"
#include "RobotManager.h"

#include <thread>
#include <memory>
#include <list>
#include "SCurve.h"

namespace MC {

	enum MovementType {
		MOVEMENT_NONE = 0,
		MOVEMENT_XY = 0x01 << 0,
		MOVEMENT_ROTATION = 0x01 << 1
	};
	   
	struct Movement
	{
		MovementType type = MOVEMENT_NONE;
		double x = 0, y = 0;
		double theta = 0;
		double distance = 0;

	};


	class MotionController {
	private:
		PIDController translationController;
		PIDController angleController;
		PIDController arcController;

		std::shared_ptr<RobotManager> robot;

		std::thread plannerThread;
		std::list<Movement> movements;
		std::mutex movementMtx;
		std::condition_variable cv;
		std::mutex cvMtx;

		void movementThread();

		SCurve::SCurveGenerator sGenerator;

		void arcControlTick(double x, double y);
		void rotationBlocking(double target, double tolerance = 0.5);

	public:
		MotionController(const std::shared_ptr<RobotManager>& _manager) : robot(_manager), sGenerator(500, 100) {}

		void init();


		void moveForward(double dist);

		void rotateTo(double theta);

		void arcToXY(double x, double y);
	};

}