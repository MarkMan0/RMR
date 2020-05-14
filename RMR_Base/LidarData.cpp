#include "LidarData.h"

#include <algorithm>

lidar::point_t lidar::Map::transform(const LidarData& data) const {
	point_t p;
	p.x = data.robPos.x + data.dist * cos(deg2rad(data.robPos.theta + data.angle - 5));
	p.y = data.robPos.y + data.dist * sin(deg2rad(data.robPos.theta + data.angle - 5));

	return p;
}

lidar::Map::Map(int _spacing) : spacing(_spacing) {
}


int lidar::Map::getClosestCoord(double d) const {
	return spacing * (static_cast<int>(d) / spacing);
}

lidar::point_t lidar::Map::getClosestPoint(const point_t& p) const {
	return point_t{ getClosestCoord(p.x), getClosestCoord(p.y) };
}

void lidar::Map::addPoint(const LidarData& data) {
	if (data.dist < 100 || data.dist > 5000) return;
	static int cnt = 0;
	++cnt;
	point_t p = transform(data);
	

	p = getClosestPoint(p);

	points[p] += 1;
};

std::vector<lidar::point_t> lidar::Map::getNeighbors(const point_t& p) const {
	std::vector<point_t> ret;
	point_t p2 = p;
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			double x = p.x + dx * spacing;
			double y = p.y + dy * spacing;
			ret.push_back(point_t(p.x + dx * spacing, p.y + dy * spacing));
		}
	}

	return ret;
}


void lidar::Map::filter() {
	if (++filtCnt < filtFreq) return;

	filtCnt = 0;

	for (auto& pair : points) {
		if (pair.second > 5*threshold || pair.second <= 1) {
			continue;
		}
		const auto neighbors = getNeighbors(pair.first);
		bool blocked = false;
		for (const auto& n : neighbors) {
			if (points[n] > threshold) {
				blocked = true;
				break;
			}
		}

		if (!blocked) {
			pair.second = 0;
		}
	}
}

bool lidar::Map::checkPoint(const Point& p, unsigned int th) const {
	point_t p2 = getClosestPoint(p);
	bool res = false;
	try {
		res = points.at(p2) > th;
	}
	catch (const std::out_of_range&) {
		res = false;
	}
	return res;
}

const lidar::Map::map_type& lidar::Map::getPoints() const
{
	return points;
}

void lidar::Map::erase() {
	for (auto& kv : points) {
		kv.second = 0;
	}
}

