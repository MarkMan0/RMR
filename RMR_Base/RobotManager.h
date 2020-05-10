#pragma once
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <optional>


#include "rplidar.h"
#include "CKobuki.h"
#include "Encoder.h"
#include "LidarData.h"
#include "Orientation.h"


class RobotManager {
public:
	typedef typename Orientation::Position Position;

private:
	
	LaserMeasurement lidarRaw;
	lidar::Map map;

	CKobuki robot;
	unsigned int dataCounter = 0;
	const std::string ipAddress;

	std::atomic<bool> robotRdy = false;

	Orientation orientation;

	bool sendCmd(const std::vector<unsigned char>& msg);

	///***** THREADS AND SOCKETS *****///
	int rob_socket = 0;
	struct sockaddr_in rob_si_me = { 0 }, rob_si_posli = { 0 }, rob_si_other = { 0 };
	unsigned int rob_slen = 0;
	SOCKET rob_s = 0, rob_recv_len = 0;
	std::thread robotThread;
	std::mutex robotMtx;
	void receiveRobotData();

	struct sockaddr_in las_si_me = { 0 }, las_si_other = { 0 }, las_si_posli = { 0 };
	SOCKET las_s = 0, las_recv_len = 0;
	unsigned int las_slen = 0;
	std::thread lidarThread;
	void receiveLidarData();

	std::mutex lidarMtx;
	std::vector<lidar::LidarData> lastMeasure;

	void processRobot();
	void processLidar();

	std::atomic<bool> stopSignal = false;

public:
	std::mutex mapMtx;

	RobotManager(const std::string& _ipAddress) : orientation(), ipAddress(_ipAddress), map(50, -500, 7000) { }
	~RobotManager();

	void init();
	void translation(int spd);
	void rotation(double spd);
	void arc(int spd, int radius);
	void arc2(int spd, double omega);
	void stop();
	bool ready() const {
		return robotRdy;
	}

	Position getPosition() {
		std::scoped_lock lcd(robotMtx);
		return orientation.getPosition();
	}

	const lidar::Map& getMap() const {
		return map;
	}

	lidar::Map& getMap() {
		return map;
	}

	bool mapRdy() const {
		return true;
	}

	std::vector<lidar::LidarData> getLastLidar();

};