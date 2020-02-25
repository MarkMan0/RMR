#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RobotManager.h"
#include <array>
#include <memory>
#include <string.h>
#include "main.h"
#include <stdlib.h>

int main() {
	using namespace std;
	std::cout << "Hello world!" << std::endl;
	
	RobotManager rob("192.168.1.12");
	//LaserMeasurement laser;
	

	rob.init();
	while (!rob.ready()) {}
	Sleep(1000);
	rob.resetEncoders();
	//rob.translation(250);
	const double kp = 2;
	const int target = 500;
	RobotManager::EncType pos = rob.getEncs();
	int cnt = 0;
	do {
		pos = rob.getEncs();
		auto act = kp * (target - pos.left);
		if (act > 300) act = 300;
		if (act < -300) act = -300;
		rob.translation((int)800);
		if (cnt > 500) cnt = 0;
		//cout << "Pos: \t" << pos.left << "\t" << pos.right << "\n";
		Sleep(10);
	} while (1);
	rob.stop();
	while (1)
	{
		pos = rob.getEncs();
		cout << "Pos: \t" << pos.left << "\t" << pos.right << "\n";
	}
	return 0;
}