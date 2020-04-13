#include "LidarData.h"

Point lidar::Map::transform(const LidarData& data) const {
	Point p;
	p.x = data.robPos.x + data.dist * cos(deg2rad(data.robPos.theta + data.angle));
	p.y = data.robPos.y + data.dist * sin(deg2rad(data.robPos.theta + data.angle));

	return p;
}

lidar::Map::Map(int _spacing, int _minmax) : spacing(_spacing), minVal(-_minmax), maxVal(_minmax), minmax(_minmax)
{
	const int numData = (2*minmax) / spacing;
	
	centerInd = numData / 2;

	points.resize(numData);
	for (auto& row : points) {
		row.resize(numData);
	}
	for (auto& row : points) {
		for (auto& data : row) {
			data = 0;
		}
	}
}

int lidar::Map::getScaledInd(double d) const {
	
	d += minmax;
	return static_cast<int>(floor(d / spacing));
}

void lidar::Map::addPoint(const LidarData& data) {
	Point p = transform(data);
	
	if (p.x < minVal || p.x > maxVal) return;
	if (p.y < minVal || p.y > maxVal) return;

	int xInd = getScaledInd(p.x);
	int yInd = getScaledInd(p.y);

	std::scoped_lock lck(mtx);
	points[xInd][yInd] += 1;
};


bool lidar::Map::checkPoint(double x, double y, int th) const {
	std::scoped_lock lck(mtx);
	return points[getScaledInd(x)][getScaledInd(y)] > th;
}