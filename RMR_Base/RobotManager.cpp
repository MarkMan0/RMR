#include "RobotManager.h"
#include "Helpers.h"
#include <thread>

void RobotManager::receiveRobotData() {
	WSADATA wsaData = { 0 };
	int iResult = 0;



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if ((rob_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{

	}

	char rob_broadcastene = 1;
	setsockopt(rob_s, SOL_SOCKET, SO_BROADCAST, &rob_broadcastene, sizeof(rob_broadcastene));
	// zero out the structure
	memset((char*)&rob_si_me, 0, sizeof(rob_si_me));

	rob_si_me.sin_family = AF_INET;
	rob_si_me.sin_port = htons(53000);
	rob_si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	rob_si_posli.sin_family = AF_INET;
	rob_si_posli.sin_port = htons(5300);
	rob_si_posli.sin_addr.s_addr = inet_addr(ipAddress.data());//inet_addr("10.0.0.1");// htonl(INADDR_BROADCAST);
	rob_slen = sizeof(rob_si_me);
	bind(rob_s, (struct sockaddr*) & rob_si_me, sizeof(rob_si_me));


	sendCmd(robot.getPIDCmd());
	Sleep(100);
	sendCmd(robot.getSoundCmd(440, 1000));
	int timeout = 200; //ms
	int intLen = sizeof(timeout);
	setsockopt(rob_s, SOL_SOCKET, SO_RCVTIMEO, (char*)(&timeout), intLen);
	while (!stopSignal)
	{

		if ((rob_recv_len = recvfrom(rob_s, (char*)(robot.robotBuff.get()), sizeof(char) * robot.buffSz, 0, (struct sockaddr*) & rob_si_other, (int*)&rob_slen)) == -1)
		{

			continue;
		}
		std::scoped_lock lck(robotMtx);
		int returnval = robot.fillData();
		if (returnval == 0)
		{
			robotRdy = true;
			processRobot();
		}


	}
	closesocket(rob_s);
}

void RobotManager::receiveLidarData() {
	WSADATA wsaData = { 0 };
	int iResult = 0;



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	las_slen = sizeof(las_si_other);
	if ((las_s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{

	}

	int las_broadcastene = 1;
	setsockopt(las_s, SOL_SOCKET, SO_BROADCAST, (char*)&las_broadcastene, sizeof(las_broadcastene));
	// zero out the structure
	memset((char*)&las_si_me, 0, sizeof(las_si_me));

	las_si_me.sin_family = AF_INET;
	las_si_me.sin_port = htons(52999);
	las_si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	las_si_posli.sin_family = AF_INET;
	las_si_posli.sin_port = htons(5299);
	las_si_posli.sin_addr.s_addr = inet_addr(ipAddress.data());//htonl(INADDR_BROADCAST);
	bind(las_s, (struct sockaddr*) & las_si_me, sizeof(las_si_me));
	char command = 0x00;
	int timeout = 200; //ms
	int intLen = sizeof(timeout);
	setsockopt(las_s, SOL_SOCKET, SO_RCVTIMEO, (char*)(&timeout), intLen);
	if (sendto(las_s, &command, sizeof(command), 0, (struct sockaddr*) & las_si_posli, las_slen) == -1)
	{

	}
	while (!stopSignal)
	{
		if ((las_recv_len = recvfrom(las_s, (char*)(lidarRaw.data.get()), sizeof(LaserData) * lidarRaw.buffSz, 0, (struct sockaddr*) & las_si_other, (int*)&las_slen)) == -1)
		{

			continue;
		}
		lidarRaw.numberOfScans = (int)(las_recv_len) / sizeof(LaserData);
		processLidar();
	}
	closesocket(las_s);
}

void RobotManager::processRobot() {
	++dataCounter;
	orientation.tick(robot.robotData.EncoderLeft, robot.robotData.EncoderRight, robot.robotData.GyroAngle);

}

void RobotManager::processLidar() {
	
	const auto pos = orientation.getPosition();

	if (pos.v != 0 || pos.omega != 0) return;		//robot not still


	if (mapMtx.try_lock()) {
		map.rawData.clear();
		map.rawData.reserve(lidarRaw.numberOfScans);
		for (int i = 0; i < lidarRaw.numberOfScans; ++i) {
			lidar::LidarData data;
			data.angle = scaleAngle(lidarRaw.data[i].scanAngle);
			data.dist = lidarRaw.data[i].scanDistance;
			data.robPos = pos;
			map.rawData.push_back(data);
			map.addPoint(data);
		}
		map.filter();
		mapMtx.unlock();
	}

}

bool RobotManager::sendCmd(const std::vector<unsigned char>& msg)
{
	return sendto(rob_s, (char*)msg.data(), (int)(sizeof(char) * msg.size()), 0, (struct sockaddr*) & rob_si_posli, sizeof(rob_si_posli)) != SOCKET_ERROR;
}

RobotManager::~RobotManager() {
	stopSignal = true;
	if(lidarThread.joinable())
		lidarThread.join();
	if(robotThread.joinable())
		robotThread.join();
}

void RobotManager::init() {
	if (!robotThread.joinable()) {
		robotThread = std::thread(&RobotManager::receiveRobotData, this);
	}
	const int misses = 15;
	int cnt = 0;
	while (!ready() && cnt < misses) {
		++cnt;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (!ready()) {
		stopSignal = true;
		robotThread.join();
		stopSignal = false;
		robotRdy = false;
		throw std::runtime_error("Couldn't connect to robot");
	}
	std::this_thread::sleep_for(std::chrono::microseconds(500));
	if (!lidarThread.joinable()) {
		lidarThread = std::thread(&RobotManager::receiveLidarData, this);
	}


	while (!ready()) {} //wait for first message
	//wait a bit
	Sleep(500);
	std::scoped_lock lck(robotMtx);
	orientation.init(robot.robotData.EncoderLeft, robot.robotData.EncoderRight, robot.robotData.GyroAngle);
}


void RobotManager::translation(int spd) {
	sendCmd(robot.getTranslationCmd(spd));
}

void RobotManager::rotation(double spd) {
	sendCmd(robot.getRotationCmd(spd));
}

void RobotManager::arc(int spd, int radius) {
	sendCmd(robot.getArcCmd(spd, radius));
}

void RobotManager::arc2(int spd, double omega) {
	sendCmd(robot.getArc2Cmd(spd, omega));
}

void RobotManager::stop() {
	sendCmd(robot.getTranslationCmd(0));
}

