#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RobotManager.h"
#include <array>
#include <memory>
#include <string.h>
#include "main.h"
#include <stdlib.h>
#include <algorithm>
#include "MotionController.h"
#include "LoopRate.h"

#include "Simulator.h"

int main() {
	using namespace std;
	std::cout << "Hello world!" << std::endl;
	
	auto robot = std::make_shared<RobotManager>("127.0.0.1");
	MC::MotionController mc(robot);

	robot->init();
	mc.init();
	mc.rotateTo(90);
	
	while (1);
	return 0;
}