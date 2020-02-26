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
	
	RobotManager rob("192.168.1.11");
	//LaserMeasurement laser;
	

	rob.init();
	
	const int kp = 5, target = 600, lo = -100, hi = 100;
	rob.translation(200);
	while (true)
	{
		double e = 1.0*target - (int) rob.encoderL().getPosition();
		double u = kp * e;
		u = u <= lo ? lo : u >= hi ? hi : u;
		//rob.translation((int)u);
		Sleep(20);

		//cout << "EncL:\t" << rob.encoderL().getPosition() << "\tEncR:\t" << rob.encoderR().getPosition() << "\tSpdL:\t" << rob.encoderL().getSpeed() << endl;
		//cout << "Angle:\t" << (rob.getAngle()) << endl;
	}

	rob.stop();
	return 0;
}