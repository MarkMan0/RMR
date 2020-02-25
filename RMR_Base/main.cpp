#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RobotManager.h"
#include <array>
#include <memory>
#include <string.h>

int main() {
	std::cout << "Hello world!" << std::endl;
	
	RobotManager rob("192.168.1.12");
	//LaserMeasurement laser;
	

	rob.init();
	while (!rob.ready()) {}
	rob.translation(250);
	while (1) {};
	return 0;
}