#pragma once
#include <chrono>
#include <thread>

class LoopRate {
private:

	using clock = std::chrono::high_resolution_clock;
	const double freq;
	const double dt;
	unsigned long long hitCnt = 0;
	using time_t = decltype(clock::now());
	const time_t start;
	time_t lastSleep;


public:
	LoopRate(double _freq) : freq(_freq), dt(1.0 / freq), start(clock::now()) {}
	
	void sleep() {
		auto begin = clock::now();
		
		++hitCnt;
		

		time_t slpUntill = start + std::chrono::milliseconds((long long)(1000 * (hitCnt * dt)));	//sleep until this time

		std::chrono::duration<double> slpLen = slpUntill - begin;
		double toSleep = slpLen.count();

		double secondsSlept = 0.0;

		while (secondsSlept < 0.9999*toSleep)
		{
			std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1> >(toSleep / 10000));
			std::chrono::duration<double> slept = clock::now() - begin;

			secondsSlept = slept.count();
		}
	}
};

