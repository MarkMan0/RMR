#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "OrientationSim.h"
#include "LidarData.h"

class Simulator {

private:
	std::ifstream robStrm, lidarStrm;

	struct PosRaw
	{
		uint32_t ts;
		uint16_t left, right;
		signed short theta;
	};

	struct Position : public OrientationSim::Position
	{
		uint32_t ts;
		Position(const OrientationSim::Position& pos, uint32_t _ts) : OrientationSim::Position(pos), ts(_ts) {}
	};

	struct LidarPair
	{
		double theta;
		double dist;
	};

	struct lidarRaw_t
	{
		uint32_t ts;
		std::vector<LidarPair> lidarPairs;
	};

	OrientationSim o;

	std::vector<lidar::Map> maps;
	std::vector<Position> position;

	friend std::ifstream& operator>>(std::ifstream&, PosRaw&);
	friend std::ifstream& operator>>(std::ifstream&, lidarRaw_t&);

	void processFiles();
	void processPosition(const std::vector<PosRaw>&);
	void processLidar(const std::vector<lidarRaw_t>&);

public:
	Simulator(const std::string& robFile, const std::string& lidarFile);

};

