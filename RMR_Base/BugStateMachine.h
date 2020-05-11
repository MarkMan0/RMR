#pragma once

#include <vector>
#include "LidarData.h"
#include "Helpers.h"

class BugStateMachine {

public:
	enum class State {
		TO_TARGET,
		WALL_ENCOUNTER,
		FOLLOWING_WALL,
	};

private:
	State state = State::TO_TARGET;
	Point target;

	bool checkWall(const std::vector<lidar::LidarData>& data) const;

	void to_TargetState(const std::vector<lidar::LidarData>&);
	void wall_encounterState(const std::vector<lidar::LidarData>&);
	void following_wallState(const std::vector<lidar::LidarData>&);

public:
	void tick(const std::vector<lidar::LidarData>& data);
	State getState() const;
	void reset();

	void setTarget(const Point&);
	Point getTarget() const;
};

