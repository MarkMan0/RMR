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

	void begin() {
		lastTime = std::chrono::steady_clock::now();
	}

	void tick(T measured) {
		T diff = 0;
		if (lastEnc > limit::max() - 10000 && measured < limit::min() + 10000) {
			//overflow
			diff = (T)(lastEnc - measured);
		}
		else if (lastEnc < limit::min() + 10000 && measured > limit::max() - 10000) {
			//underflow
			diff = -1 * (T)(lastEnc - measured);

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
