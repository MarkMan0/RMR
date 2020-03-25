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
	
	Simulator sim(std::string("robotdata.txt"), std::string("lidardata.txt"));
	
	while (1);
	return 0;
}