#pragma once

#include <vector>
#include "LidarData.h"
#include "Helpers.h"

class BugStateMachine {

private:
	enum class State {
		TO_TARGET,
		WALL_ENCOUNTER,
		FOLLOWING_WALL,
	};

	State state;
	Point target;

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

