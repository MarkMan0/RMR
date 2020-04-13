#pragma once

#include <vector>
#include <mutex>
#include "Helpers.h"
#include "Orientation.h"

namespace lidar {

	struct LidarData {
		Orientation::Position robPos;
		double angle, dist;
	};

	class Map {
	public:
		using map_type = std::vector< std::vector<unsigned int>>;
		
	private:
		map_type points;
		
		const unsigned int minmax;
		int centerInd;
		Point transform(const LidarData&) const;
		int getScaledInd(double d) const;
	public:

		mutable std::mutex mtx;
		std::vector<LidarData> rawData;
		Map(int _spacing, int _minmax);
		void addPoint(const LidarData& point);
		bool checkPoint(double x, double y, int th = 1) const;
		const int minVal, maxVal;
		const int spacing;
	};
}