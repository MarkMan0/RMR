#pragma once

#include <vector>

namespace lidar {
	struct LidarData
	{
		double x, y;
	};

	class Map {
	private:
	public:
		std::vector<LidarData> points;
		void addPoint(const LidarData& point) {
			points.push_back(point);

		}
	};
}