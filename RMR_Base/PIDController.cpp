#include "PIDController.h"

template<class T>
static inline T clamp(T what, T bot, T top) {
	return what <= bot ? bot : what >= top ? top : what;
}


PIDController& PIDController::setKp(double k)
{
	this->Kp = k;
	return *this;
}

PIDController& PIDController::setKi(double k)
{
	this->Ki = k;
	return *this;
}

PIDController& PIDController::setKd(double k)
{
	this->Kd = k;
	return *this;
}

PIDController& PIDController::setUpper(double d)
{
	this->upper = d;
	return *this;
}

PIDController& PIDController::setLower(double d)
{
	this->lower = d;
	return *this;
}

PIDController& PIDController::setSampleT(double t)
{
	sampleT = t;
	return *this;
}

void PIDController::enable(bool b /*=true*/) {
	enabled = b;
}

double PIDController::tick(double e)
{
	if (!enabled) {
		integral = 0;
		last = e;
		return 0.0;
	}

	integral += Ki * e;
	integral = clamp(integral, lower, upper);

	double derivative = Kd * (e - last) / sampleT;
	last = e;

	return clamp(Kp * e + integral + derivative, lower, upper);
}
