#pragma once

#include <list>
#include <chrono>
#include <optional>

namespace scurve {

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
		std::optional<double> expectedVel;
		std::optional<double> expectedPos;

		AccPoint(double _t, double _val, std::optional<double> _expVel = std::nullopt, std::optional<double> _expPos = std::nullopt) 
			: t(_t), val(_val), expectedVel(_expVel), expectedPos(_expPos) {}
	};

	class SCurve {
		friend class SCurveGenerator;
	private:
		std::list<AccPoint> accPoints;
		double direction = 0;
		void addPoint(double _t, double _val, std::optional<double> _expVel = std::nullopt, std::optional<double> _expPos = std::nullopt);
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