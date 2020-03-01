#pragma once
#include <math.h>

static bool compareDoubles(double d1, double d2, double delta = 0.01) {
	return (abs(d1 - d2) < abs(delta));
}