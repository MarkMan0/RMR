#pragma once
#include <limits>

class PIDController {

private:
	double Kp = 0, Ki = 0, Kd = 0;
	double last = 0, integral = 0;

	double upper = std::numeric_limits<double>::max(), lower = std::numeric_limits<double>::lowest();

	double sampleT= 1;

public:
	PIDController() = default;

	PIDController& setKp(double k);
	PIDController& setKi(double k);
	PIDController& setKd(double k);
	PIDController& setUpper(double d);
	PIDController& setLower(double d);
	PIDController& setSampleT(double t);

	double tick(double e);
};

