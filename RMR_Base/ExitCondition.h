#pragma once
#include <cmath>

class ExitCondition {
private:
	const int ticks;
	int ticksDone;
	const double delta;

public:
	bool check(double e) {
		if (abs(e) < delta) {
			++ticksDone;
		}
		else {
			ticksDone = 0;
		}

		return ticksDone > ticks;

	}

	ExitCondition(int ticks, double delta) : ticks(ticks), delta(delta), ticksDone(0) {}


};