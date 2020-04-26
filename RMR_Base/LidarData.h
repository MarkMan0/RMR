#pragma once

#include <vector>
#include <mutex>
#include <map>
#include "Helpers.h"
#include "Orientation.h"

namespace lidar {
	using point_t = BasePoint<int>;

	struct LidarData {
		Orientation::Position robPos;
		double angle, dist;
	};

	class Map {
	public:
		using map_type = std::map<point_t, unsigned int>;
		
	private:
		map_type points;
		
		int centerInd;
		point_t transform(const LidarData&) const;
		int getClosestCoord(double d) const;
		point_t getClosestPoint(const point_t& p) const;
		int filtCnt = 0;
		const int filtFreq = 10;
	public:

		const int threshold = 10;
		std::vector<LidarData> rawData;
		Map(int _spacing, int _min, int _max);
		Map(int _spacing, int _minmax);
		void addPoint(const LidarData& point);
		std::vector<point_t> getNeighbors(const point_t& p) const;
		void filter();
		bool checkPoint(const Point& p, unsigned int th = 1) const;
		const map_type& getPoints() const;
		const int minVal, maxVal;
		const int spacing;
		void erase();
	};
}