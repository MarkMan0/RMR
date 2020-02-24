#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RobotManager.h"
#include <array>
#include <memory>
#include <string.h>

int main() {
	std::cout << "Hello world!" << std::endl;
	{
		RobotManager rob("");
		LaserMeasurement laser;
	}
	std::unique_ptr<char[]> buff = std::make_unique<char[]>(100);

	strcpy(buff.get(), "It's working");

	printf("%s\n", buff.get());

	return 0;
}