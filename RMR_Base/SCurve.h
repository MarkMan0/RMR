#pragma once

#include <list>
#include <chrono>

namespace SCurve {

	using clock = std::chrono::high_resolution_clock;
	using time_point = decltype(clock::now());

	struct Point {
		double x, y;
		Point(double _x, double _y) : x(_x), y(_y) {}
	};

	struct AccPoint
	{
		double t;
		double val;
		double expectedVel = -1;
	};

	class SCurve {
		friend class SCurveGenerator;
	private:
		std::list<AccPoint> accPoints;
		double direction = 0;
		void addPoint(double t, double val, double expected = -1);
		time_point startPoint, lastCall;
		bool valid = false;

		double velocity = 0, position = 0;

		Point from, to;
	public:
		SCurve(Point _from, Point _to);
		bool pointNow(Point& dest);
		void begin();
	};



	class SCurveGenerator {

	private:
		const double maxAcc, maxVel;


	public:
		SCurveGenerator(double _maxVel, double _maxAcc) : maxAcc(_maxAcc), maxVel(_maxVel) {}
		SCurve createCurve(Point from, Point to);

	};


}