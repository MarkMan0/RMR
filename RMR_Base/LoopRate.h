#pragma once
#include <chrono>
#include <thread>

class LoopRate {
private:

	using clock = std::chrono::high_resolution_clock;
	double freq;
	decltype(clock::now()) lastSleep;


public:
	LoopRate(double _freq): freq(_freq) {
		lastSleep = clock::now();
	}
	
	void sleep() {
		auto begin = clock::now();

		std::chrono::duration<double> sinceLast = begin - lastSleep;

		double toSleep = 1.0 / freq - sinceLast.count();

		double secondsSlept = 0.0;

		while (secondsSlept < 0.9999*toSleep)
		{
			std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1> >(toSleep / 10000));
			std::chrono::duration<double> slept = clock::now() - begin;

			secondsSlept = slept.count();
		}
		lastSleep = clock::now();
	}
};

