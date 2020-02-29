#pragma once
#include <chrono>
#include <limits>

//Class to manage encoders
template<class T, class D>
class Encoder {

private:
	using limit = std::numeric_limits<T>;
	T lastEnc = 0;
	decltype(std::chrono::steady_clock::now()) lastTime;
	D position = 0, speed = 0, zeroVal = 0;
	const double tick2mm;

public:
	Encoder() : tick2mm(1) {}
	Encoder(double _tick2mm) : tick2mm(_tick2mm) {};

	void begin(T val) {
		lastTime = std::chrono::steady_clock::now();
		lastEnc = val;
		zeroVal = val * tick2mm;
		position = zeroVal;
	}

	D tick(T measured) {
		int diff = 0;
		if (lastEnc > limit::max() - 1000 && measured < limit::min() + 1000) {
			//overflow
			diff = (limit::max() - lastEnc) + (limit::min() - measured) + 1;
		}
		else if (lastEnc < limit::min() + 1000 && measured > limit::max() - 1000) {
			//underflow
			//diff is the absolute distance between min() and last + between max() and now
			diff = (limit::min() - lastEnc) + (limit::max() - measured) + 1;
			diff *= -1;
		}
		else {
			//normal behaviour
			diff = measured - lastEnc;
		}

		lastEnc = measured;
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> dt = (now - lastTime);
		lastTime = now;
		speed = (diff * tick2mm) / (dt.count()); // mm/s
		position += (diff * tick2mm);
		return (diff * tick2mm);

	}

	D getSpeed() const {
		return speed;
	}
	D getPosition() const {
		return position - zeroVal;
	}
	void zeroNow() {
		zeroVal = position;
	}
};
