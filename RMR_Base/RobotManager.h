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
#include "Orientation.h"


class RobotManager {
public:
	typedef typename Orientation::Position Position;

private:
	
	LaserMeasurement lidarRaw;

	std::vector<LidarPoint> lidarPoints;

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

	void processRobot();
	void processLidar();
	std::optional<LidarPoint> applyTransform(const LaserData& data, const Orientation::Position& pos);


public:
	RobotManager(const std::string& _ipAddress) : orientation(230, 35, robot.tickToMeter*1000), ipAddress(_ipAddress) { }

	void init();
	void translation(int spd);
	void rotation(double spd);
	void arc(int spd, int radius);
	void arc2(int spd, double omega);
	void stop();
	bool ready() const {
		return robotRdy;
	}
	TKobukiData getData() {
		std::scoped_lock lck(robotMtx);
		return robot.robotData;
	}

	Orientation::EncType encoderL() {
		std::scoped_lock lck(robotMtx);
		return orientation.getLeft();
	}
	Orientation::EncType encoderR() {
		std::scoped_lock lck(robotMtx);
		return orientation.getRight();
	}

	double getAngle() {
		std::scoped_lock lck(robotMtx);
		return orientation.getTheta();
	}

	Position getPosition() {
		std::scoped_lock lcd(robotMtx);
		return orientation.getPosition();
	}
	
};