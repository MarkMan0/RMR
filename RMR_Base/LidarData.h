#pragma once

#include <vector>
#include <mutex>
#include <map>
#include "Helpers.h"
#include "Orientation.h"

namespace lidar {

	struct LidarData {
		Orientation::Position robPos;
		double angle, dist;
	};

	class Map {
	public:
		using map_type = std::map<Point, unsigned int>;
		
	private:
		map_type points;
		
		int centerInd;
		Point transform(const LidarData&) const;
		int getClosestCoord(double d) const;
		Point getClosestPoint(const Point& p) const;
	public:

		mutable std::mutex mtx;
		std::vector<LidarData> rawData;
		Map(int _spacing, int _min, int _max);
		Map(int _spacing, int _minmax);
		void addPoint(const LidarData& point);
		bool checkPoint(const Point& p, unsigned int th = 1) const;
		const map_type& getMap() const;
		const int minVal, maxVal;
		const int spacing;
		void erase();
	};
}