#include "Simulator.h"
#include <exception>

#include <cstdint>
#include <iostream>

#include "Helpers.h"


std::ifstream& operator>>(std::ifstream& in, Simulator::PosRaw& data) {
	//reads one line into a struct
	in >> data.ts;
	in >> data.left;
	in >> data.right;
	in >> data.theta;

	return in;
}

std::ifstream& operator>>(std::ifstream& in, Simulator::lidarRaw_t& data) {
	//reads a packet of data
	/*Data structure:
		timestamp, numberOfReadings
		distances....
		theta....
	*/

	in >> data.ts;
	int numOfReadings = 0;
	in >> numOfReadings;

	data.lidarPairs.resize(numOfReadings);

	for (int i = 0; i < numOfReadings; ++i) {	
		in >> data.lidarPairs[i].dist;
	}
	for (int i = 0; i < numOfReadings; ++i) {
		in >> data.lidarPairs[i].theta;
		data.lidarPairs[i].theta *= -1;		//TODO: verify
	}

	return in;
}

void Simulator::processFiles() {

	std::vector<PosRaw> posRaw;
	PosRaw data{ 0 };
	while (robStrm >> data) {
		posRaw.push_back(data);
	}
	processPosition(posRaw);

	std::vector<lidarRaw_t> lidarRaw;
	lidarRaw_t raw;
	while (lidarStrm >> raw)
	{
		lidarRaw.push_back(raw);
	}

	processLidar(lidarRaw);
}

void Simulator::processPosition(const std::vector<PosRaw>& raw) {
	auto beg = raw.cbegin();
	auto end = raw.cend();
	const auto epoch = std::chrono::time_point<std::chrono::high_resolution_clock>();


	if (beg != end) {
		auto timestamp = epoch + std::chrono::milliseconds(beg->ts);
		o.init(beg->left, beg->right, beg->theta, timestamp);
	}
	position.emplace_back(o.getPosition(), beg->ts);
	++beg;

	while (beg != end) {
		auto timestamp = epoch + std::chrono::milliseconds(beg->ts);
		o.tick(beg->left, beg->right, beg->theta, timestamp);
		position.emplace_back(o.getPosition(), beg->ts);
		++beg;
	}
}

void Simulator::processLidar(const std::vector<lidarRaw_t>& raw) {
	
	auto posBeg = position.cbegin();
	auto posEnd = position.cend();
	int cnt = 0;
	for (const auto& data : raw) {
		uint32_t ts = data.ts;
		while (posBeg != (posEnd - 1)) {
			if (posBeg->ts < ts && (posBeg + 1)->ts > ts) {
				//the closest point is this one or the next
				if (abs(1.0 * ts - posBeg->ts) < abs(1.0 * ts - (posBeg + 1)->ts)) {
					//this one is closer, choose this
					break;
				}
				else {
					//next is closer, choose that
					++posBeg;
					break;
				}
			}
			++posBeg;
		}
		const auto& pos = *posBeg;
		lidar::Map map;
		for (const auto& measurement : data.lidarPairs) {
			constexpr int maxDist = 2000;	// 2 meters
			if (measurement.dist > maxDist) {
				continue;
			}
			const double x = pos.x + measurement.dist * cos(deg2rad(pos.theta + measurement.theta));
			const double y = pos.y + measurement.dist * sin(deg2rad(pos.theta + measurement.theta));
			lidar::LidarData data;
			data.x = x;
			data.y = y;
			map.addPoint(data);
		}

		maps.push_back(map);
	}

}

Simulator::Simulator(const std::string& robFile, const std::string& lidarFile) : o(230, 35, 0.000085292090497737556558 * 1000) {
	robStrm.open(robFile, std::ifstream::in);
	lidarStrm.open(lidarFile, std::ifstream::in);

	if (!robStrm.is_open()) {
		throw std::runtime_error("Robot data file not found");
	}
	if (!lidarStrm.is_open()) {
		throw std::runtime_error("Lidar data file not found");
	}
	processFiles();
}
