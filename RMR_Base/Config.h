#pragma once


namespace config {
	struct pid_param_t {
		double p = 0, i = 0, d = 0;
		double lower = 0, upper = 0;
		double sampleT = 1;
	};


	constexpr double startX = 1000;
	constexpr double startY = 1000;

	constexpr pid_param_t transCont{ 2, 0, 0.5, -500, 500, 1.0/50.0 };
	constexpr pid_param_t angleCont{ 0.1, 0, 0.05, -1, 1, 1.0 / 50.0 };
	constexpr pid_param_t arcCont{ 0.1, 0, 0.03, -0.3, 0.3, 1.0 / 50.0 };

	namespace rob {
		constexpr double d = 230,
			r = 35,
			tick2mm = 0.000085292090497737556558 * 1000.0,
			tick2deg = 1.0 / 100.0,
			thetaMax = 18000,
			thetaMin = -17999;
	}
}