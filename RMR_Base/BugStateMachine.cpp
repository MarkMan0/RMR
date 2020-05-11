#include "BugStateMachine.h"

#include <cmath>

bool BugStateMachine::checkWall(const std::vector<lidar::LidarData>& data) const {
	int cnt = 0;
	for (const auto& d : data) {
		if (std::abs(d.angle) < 30 && d.dist < 500) {
			++cnt;
		}
	}
	if (cnt > 5) {
		return true;
	}

	return false;
}

void BugStateMachine::to_TargetState(const std::vector<lidar::LidarData>& data) {

	if (checkWall(data)) {
		state = State::WALL_ENCOUNTER;
	}
	
}

void BugStateMachine::wall_encounterState(const std::vector<lidar::LidarData>& data) {
	state = State::FOLLOWING_WALL;
}

void BugStateMachine::following_wallState(const std::vector<lidar::LidarData>& data) {

	auto pos = data[0].robPos;
	double toTarget = pos.p.dirTo(target);
	
	if (abs(scaleAngle(pos.theta) - rad2deg(toTarget)) < 5) {
		state = State::TO_TARGET;
	}
	if (checkWall(data)) {
		state = State::WALL_ENCOUNTER;
	}
}

void BugStateMachine::tick(const std::vector<lidar::LidarData>& data) {

	switch (state)
	{
	case BugStateMachine::State::TO_TARGET:
		to_TargetState(data);
		break;
	case BugStateMachine::State::WALL_ENCOUNTER:
		wall_encounterState(data);
		break;
	case BugStateMachine::State::FOLLOWING_WALL:
		following_wallState(data);
		break;
	default:
		break;
	}
}

BugStateMachine::State BugStateMachine::getState() const
{
	return state;
}

void BugStateMachine::reset() {
	state = State::TO_TARGET;
}

void BugStateMachine::setTarget(const Point& p) {
	target = p;
	reset();
}

Point BugStateMachine::getTarget() const
{
	return target;
}
