#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RobotManager.h"
#include <array>
#include <memory>
#include <string.h>
#include "main.h"
#include <stdlib.h>
#include <algorithm>

int main() {
	using namespace std;
	std::cout << "Hello world!" << std::endl;
	
	RobotManager rob("192.168.1.12");
	//LaserMeasurement laser;
	

	rob.init();
	
	const int kp = 2, target = 300, lo = -500, hi = 500;
	while (true)
	{
		double e = target - (int) rob.encoderL().getPosition();
		double u = kp * e;
		u = u <= lo ? lo : u >= hi ? hi : u;
		rob.translation((int)u);
		Sleep(200);
	}

	rob.stop();
	return 0;
}