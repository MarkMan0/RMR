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
	
	std::shared_ptr<RobotManager> rob = std::make_shared<RobotManager>("192.168.1.11");
	rob->init();
	
	MC::MotionController mc(rob);
	mc.init();

	mc.arcToXY(2000, -1000);
	mc.arcToXY(1000, -3000);
	mc.arcToXY(0, 0);

	rob->stop();
	while (1);
	return 0;
}