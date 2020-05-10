#pragma once
#include "PIDController.h"
#include "RobotManager.h"

#include <thread>
#include <memory>
#include <list>
#include "SCurve.h"
#include "MazeSolver.h"
#include "BugStateMachine.h"

namespace MC {

	enum class MovementType : uint8_t {
		MOVEMENT_NONE = 0x01 << 0,
		MOVEMENT_XY = 0x01 << 1,
		MOVEMENT_ROTATION = 0x01 << 2,
		MOVEMENT_PAUSE = 0x01 << 3,
		MOVEMENT_BUG = 0x01 << 4,
	};

	enum class PlannerMode : uint8_t {
		LINEAR, 
		BUG_ALGORITHM,
		USE_MAP,
		CURRENT
	};

	bool operator&(const MovementType& a, const MovementType& b);
	   
	struct Movement
	{
		MovementType type = MovementType::MOVEMENT_NONE;
		double x = 0, y = 0;
		double theta = 0;
		double distance = 0;
		unsigned int waitT;
	};


	class MotionController {
	private:
		PIDController translationController;
		PIDController angleController;
		PIDController arcController;
		PIDController wallController;

		using BSM = BugStateMachine;
		BSM bsm;
		
		std::shared_ptr<RobotManager> robot;

		std::thread plannerThread;
		std::list<Movement> movements;
		std::mutex movementMtx;
		std::condition_variable cv;
		std::mutex cvMtx;

		void movementThread();
		std::atomic<bool> stopSignal = false;

		scurve::SCurveGenerator sGenerator;

		maze::MazeSolver solver;

		void arcControlTick(double x, double y);
		void wallFollowContTick(double dist);
		void rotationBlocking(double target, double tolerance = 1);
		void arcToXYBlocking(double x, double y);
		double distToWall(const std::vector<lidar::LidarData>& data) const;
		void bugBlocking(const Point&);
		void planOnMap(const Point& p);
		void arcToXY(double x, double y);
	public:
		PlannerMode plannerMode = PlannerMode::LINEAR;

		MotionController(const std::shared_ptr<RobotManager>& _manager) : robot(_manager), sGenerator(500, 100) {}
		~MotionController();
		void init();

		const maze::MazeSolver& getSolver() const;

		void moveForward(double dist);
		void rotateTo(double theta);
		void addPause(unsigned int ms);

		void bugToXY(const Point& p);

		void moveToPoint(const Point& p, PlannerMode mode = PlannerMode::CURRENT);
	};

}