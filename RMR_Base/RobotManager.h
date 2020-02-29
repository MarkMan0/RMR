#pragma once
#include "rplidar.h"
#include "CKobuki.h"
#include <string>
#include <vector>
#include <memory>
#include <thread>

#include <cstdint>
#include "Encoder.h"
#include "Orientation.h"
#include <atomic>

class RobotManager {
private:
	
	LaserMeasurement measure;

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
	int rob_s = 0, rob_recv_len = 0;
	std::thread robotThread;
	void receiveRobotData();

	struct sockaddr_in las_si_me = { 0 }, las_si_other = { 0 }, las_si_posli = { 0 };
	int las_s = 0, las_recv_len = 0;
	unsigned int las_slen = 0;
	std::thread lidarThread;
	void receiveLidarData();

	void processRobot();
	void processLidar();


public:
	RobotManager(const std::string& _ipAddress) : orientation(230, 35, robot.tickToMeter*1000), ipAddress(_ipAddress) { }

	void init();
	void translation(int spd);
	void rotation(double spd);
	void arc(int spd, int radius);
	void stop();
	bool ready() const {
		return robotRdy;
	}
	const TKobukiData& getData() const {
		return robot.robotData;
	}

	const Orientation::EncType& encoderL() const {
		return orientation.getLeft();
	}
	const Orientation::EncType& encoderR() const {
		return orientation.getRight();
	}

	double getAngle() const {
		return orientation.getTheta();
	}
	
};