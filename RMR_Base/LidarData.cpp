#include "LidarData.h"

Point lidar::Map::transform(const LidarData& data) const {
	Point p;
	p.x = data.robPos.x + data.dist * cos(deg2rad(data.robPos.theta + data.angle - 5));
	p.y = data.robPos.y + data.dist * sin(deg2rad(data.robPos.theta + data.angle - 5));

	return p;
}

lidar::Map::Map(int _spacing, int _min, int _max) : spacing(_spacing), minVal(_min), maxVal(_max) {
	const int numData = (maxVal - minVal) / spacing;

	centerInd = numData / 2;

	for (int x = minVal; x < maxVal; x += spacing) {
		for (int y = minVal; y < maxVal; y += spacing) {
			Point p(x, y);
			points[p] = 0;
		}
	}
}

lidar::Map::Map(int _spacing, int _minmax) : Map(spacing, -_minmax, _minmax) { }

int lidar::Map::getClosestCoord(double d) const {
	return spacing * (static_cast<int>(d) / spacing);
}

Point lidar::Map::getClosestPoint(const Point& p) const {
	return Point{ 1.0*getClosestCoord(p.x), 1.0*getClosestCoord(p.y) };
}

void lidar::Map::addPoint(const LidarData& data) {
	if (data.dist < 100 || data.dist > 5000) return;
	
	Point p = transform(data);
	
	if (p.x < minVal || p.x > maxVal) return;
	if (p.y < minVal || p.y > maxVal) return;

	p = getClosestPoint(p);


#ifndef TESTING
	std::scoped_lock lck(mtx);
#endif // !TESTING

	points[p] += 1;
};


bool lidar::Map::checkPoint(const Point& p, unsigned int th) const {
#ifndef TESTING
	std::scoped_lock lck(mtx);
#endif // !TESTING
	Point p2 = getClosestPoint(p);
	bool res = false;
	try {
		res = points.at(p2) > th;
	}
	catch (const std::out_of_range&) {
		res = false;
	}
	return res;
}

const lidar::Map::map_type& lidar::Map::getMap() const
{
	return points;
}

void lidar::Map::erase() {
	for (auto& kv : points) {
		kv.second = 0;
	}
}
