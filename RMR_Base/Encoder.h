#pragma once
#include <chrono>
#include <limits>
#undef min
#undef max
//Class to manage encoders
template<class T, class D>
class Encoder {
public:
	using clock_t = std::chrono::steady_clock;
	using now_t = decltype(clock_t::now());
private:
	using limit = std::numeric_limits<T>;
	const T minVal = limit::min();
	const T maxVal = limit::max();

	T lastEnc = 0;
	now_t lastTime;
	D position = 0, speed = 0;
	const double mmPerTick;

public:


	Encoder() : mmPerTick(1) {}
	Encoder(double _mmPerTick) : mmPerTick(_mmPerTick) {};
	Encoder(double _mmPerTick, T _minVal, T _maxVal) : mmPerTick(_mmPerTick), minVal(_minVal), maxVal(_maxVal) {}

	void begin(T val, now_t now = clock_t::now()) {
		lastTime = now;
		lastEnc = val;
		position = 0.0;
	}

	D tick(T measured, now_t now = clock_t::now()) {
		int diff = 0;
		if (lastEnc > maxVal - 1000 && measured < minVal + 1000) {
			//overflow
			diff = abs(maxVal - lastEnc) + abs(minVal - measured) + 1;
		}
		else if (lastEnc < minVal + 1000 && measured > maxVal - 1000) {
			//underflow
			//diff is the absolute distance between min() and last + between max() and now
			diff = abs(minVal - lastEnc) + abs(maxVal - measured) + 1;
			diff *= -1;
		}
		else {
			//normal behaviour
			diff = measured - lastEnc;
		}

		lastEnc = measured;
		std::chrono::duration<float> dt = (now - lastTime);
		lastTime = now;
		speed = (diff * mmPerTick) / (dt.count()); // mm/s
		position += (diff * mmPerTick);
		return (diff * mmPerTick);

	}

	D getSpeed() const {
		return speed;
	}
	D getPosition() const {
		return position;
	}
};
