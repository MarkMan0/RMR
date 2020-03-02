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

int main() {
	using namespace std;
	std::cout << "Hello world!" << std::endl;
	
	std::shared_ptr<RobotManager> rob = std::make_shared<RobotManager>("192.168.1.12");
	rob->init();
	
	MotionController mc(rob);
	mc.init();

	mc.moveForward(1000);
	mc.rotateTo(-90);
	mc.moveForward(1000);
	mc.rotateTo(-180);
	mc.moveForward(1000);
	mc.rotateTo(-270);
	mc.moveForward(1000);
	mc.rotateTo(-360);
	
	rob->stop();
	while (1);
	return 0;
}