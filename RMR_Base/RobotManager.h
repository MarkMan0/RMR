#pragma once
#include "rplidar.h"
#include "CKobuki.h"
#include <string>
//#include <Windows.h>
#include <vector>
#include <memory>

class RobotManager {
private:
	
	LaserMeasurement measure;

	CKobuki robot;
	unsigned int dataCounter = 0;
	const std::string ipAddress;

	bool robotRdy = false;


	int rob_socket = 0;
	struct sockaddr_in rob_si_me = { 0 }, rob_si_posli = { 0 }, rob_si_other = { 0 };
	unsigned int rob_slen = 0;
	int rob_s = 0, rob_recv_len = 0;

	HANDLE robotthreadHandle = 0; // handle na vlakno
	DWORD robotthreadID = 0;  // id vlakna
	void receiveRobotData();
	static DWORD WINAPI robotUDPThread(void* ptrToThis) {
		((RobotManager*)ptrToThis)->receiveRobotData();
		return 0;
	}

	struct sockaddr_in las_si_me = { 0 }, las_si_other = { 0 }, las_si_posli = { 0 };
	int las_s = 0, las_recv_len = 0;
	unsigned int las_slen = 0;

	HANDLE laserthreadHandle = 0; // handle na vlakno
	DWORD laserthreadID = 0;  // id vlakna
	void receiveLidarData();
	static DWORD WINAPI lidarUDPThread(void* ptrToThis) {
		((RobotManager*)ptrToThis)->receiveLidarData();
		return 0;
	}

	void processRobot();
	void processLidar(const LaserMeasurement&);

	bool sendCmd(const std::vector<unsigned char>& msg);


public:
	RobotManager(const std::string& _ipAddress) : ipAddress(_ipAddress) { }

	void init();
	void translation(int spd);
	void rotation(double spd);
	void stop();
	bool ready() const {
		return robotRdy;
	}
};