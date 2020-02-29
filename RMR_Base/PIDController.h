#pragma once
class PIDController {

private:
	double Kp, Ki, Kd;
	double last = 0, integral = 0;

	double upper, lower;

	double sampleT;

	bool enabled;

public:
	PIDController() = default;

	PIDController& setKp(double k);
	PIDController& setKi(double k);
	PIDController& setKd(double k);
	PIDController& setUpper(double d);
	PIDController& setLower(double d);
	PIDController& setSampleT(double t);
	void enable(bool b = true);

	double tick(double e);
};

