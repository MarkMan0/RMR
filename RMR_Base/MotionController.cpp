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

	while (!stopSignal) {
		if (!movements.empty()) {
			const auto target = movements.front();
			auto pos = robot->getPosition();

			if (target.type & MovementType::MOVEMENT_XY) {

				double pointAngleNow = rad2deg(atan2(target.y - pos.y, target.x - pos.x));
				double targetTheta = getClosestTargetAngle(pos.theta, pointAngleNow);
				rotationBlocking(targetTheta, 10);	//rotate towards target

				arcToXYBlocking(target.x, target.y);


			}
			if (target.type & MovementType::MOVEMENT_BUG) {
				bugBlocking(Point(target.x, target.y));
			}
			if (target.type & MovementType::MOVEMENT_ROTATION) {
				rotationBlocking(target.theta);
			}
			if (target.type & MovementType::MOVEMENT_PAUSE) {
				auto ms = target.waitT;
				robot->stop();
				std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			}
			robot->stop();
			movements.pop_front();
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		if (movements.empty()) {
			std::unique_lock lck(cvMtx);
			cv.wait(lck, [this]()->bool {return (!(movements.empty()) || stopSignal); });
		}
	}
}

MC::MotionController::~MotionController() {
	stopSignal = true;
	cv.notify_all();
	if(plannerThread.joinable())
		plannerThread.join();
}

void MC::MotionController::init() {
	translationController.setParams(config::transCont);

	angleController.setParams(config::angleCont);

	arcController.setParams(config::arcCont);

	wallController.setParams(config::wallCont);

	if (!plannerThread.joinable()) {
		plannerThread = std::thread(&MotionController::movementThread, this);
	}
}

const maze::MazeSolver& MC::MotionController::getSolver() const
{
	return solver;
}

void MC::MotionController::rotationBlocking(double target, double tolerance)
{
	bool done = false;

	ExitCondition cond(25, tolerance);

	LoopRate rate(50);
	while (!done && !stopSignal) {
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

void MC::MotionController::wallFollowContTick(double dist) {
	int spd = 300;
	double o = wallController.tick(dist);
	double radius = spd / o;

	if (isnan(radius) || isinf(radius)) {
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
	
	while (!cond.check(getDist()) && !stopSignal) 	{
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

double MC::MotionController::distToWall(const std::vector<lidar::LidarData>& data) const {
	const double target = 90;
	auto ld = data[0];
	for (const auto& d : data) {
		if (std::abs(target - d.angle) < std::abs(target - ld.angle) ) {
			ld = d;
		}
	}

	return ld.dist;
}


void MC::MotionController::bugBlocking(const Point& p) {
	auto pos = robot->getPosition();
	auto getDist = [&pos, &p]() {
		double d = pos.p.dist(p);
		return d; 
	};

	LoopRate rate(50);
	ExitCondition cond(0, 100);
	bool change = true;
	while (!cond.check(getDist()) && !stopSignal) {

		auto state = bsm.getState();
		auto data = robot->getLastLidar();
		pos = robot->getPosition();
		switch (state)
		{
		case BugStateMachine::State::TO_TARGET:
			if (change) {
				rotationBlocking(rad2deg(pos.p.dirTo(p)), 10);
				change = false;
			}
			arcControlTick(p.x, p.y);
			break;
		case BugStateMachine::State::WALL_ENCOUNTER:
			change = true;
			rotationBlocking(pos.theta + 90, 5);
			break;
		case BugStateMachine::State::FOLLOWING_WALL: 
			change = true;
			wallFollowContTick(distToWall(data));
			break;
		default:
			break;
		}
		bsm.tick(data);
		rate.sleep();
	}
	robot->stop();
}

void MC::MotionController::planOnMap(const Point& to) {
	{
		std::scoped_lock lck(robot->mapMtx);
		solver.loadMaze(robot->getMap());
	}
	Point p;
	const auto pos = robot->getPosition();
	p.x = pos.x;
	p.y = pos.y;


	solver.setSource(p);

	solver.setTarger(to);
	solver.dijkstra();

	for (const auto& p : solver.getSolution()) {
		arcToXY(p.x, p.y);
	}
}

void MC::MotionController::arcToXY(double x, double y) {

	Movement mv;
	mv.type = MovementType::MOVEMENT_XY;
	mv.x = x;
	mv.y = y;

	movements.push_back(mv);
	cv.notify_all();
}

void MC::MotionController::addPause(unsigned int ms) {
	Movement mv;
	mv.type = MovementType::MOVEMENT_PAUSE;
	mv.waitT = ms;
	movements.push_back(mv);
	cv.notify_all();
}

void MC::MotionController::bugToXY(const Point& p) {
	Movement mv;
	mv.type = MovementType::MOVEMENT_BUG;
	mv.x = p.x;
	mv.y = p.y;
	movements.push_back(mv);
	cv.notify_all();
}

void MC::MotionController::moveToPoint(const Point& p, PlannerMode mode) {
	
	if (mode == PlannerMode::CURRENT) {
		mode = plannerMode;
	}

	if (mode == PlannerMode::LINEAR) {
		arcToXY(p.x, p.y);
	}
	else if (mode == PlannerMode::BUG_ALGORITHM){
		bugToXY(p);
	}
	else if (mode == PlannerMode::USE_MAP) {
		planOnMap(p);
	}

}
