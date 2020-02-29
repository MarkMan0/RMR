#include "pch.h"
#include "CppUnitTest.h"

#include "../RMR_Base/Orientation.h"
#include "../RMR_Base/Orientation.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace OrientationTests {

	TEST_CLASS(OrientationTests) {
	public:

		TEST_METHOD(TestForwardMovement) {
			/* Forward movement conditions:
				theta = 0, x = 0, y = 0
			  Expected behaviour:
				theta = 0, x >> 0, y ~= 0
			*/

			Orientation o(1, 1, 1);

			o.init();

			unsigned short xyTick = 0;
			signed short angleTick = 0;

			for (int i = 0; i < 100; ++i) {
				//move 100 ticks with both wheels
				++xyTick;
				o.tick(xyTick, xyTick, angleTick);
			}
			Assert::AreEqual(1.0 * xyTick, o.getPosition().x, L"Coordinate X is not as expected");
			Assert::AreEqual(0.0, o.getPosition().y, L"Coordinate Y is not as expected");
			Assert::AreEqual(0.0, o.getPosition().theta, L"Coordinate THETA is not as expected");

		}

		TEST_METHOD(TestReverseMovement) {
			Orientation o(1, 1, 1);

			o.init();

			unsigned short xyTick = 100;
			signed short angleTick = 0;
			int dist = xyTick;
			for (int i = 0; i < 500; ++i) {
				//move 100 ticks with both wheels
				--xyTick;
				o.tick(xyTick, xyTick, angleTick);
				--dist;
			}
			Assert::AreEqual(1.0 * dist, o.getPosition().x, L"Coordinate X is not as expected");
			Assert::AreEqual(0.0, o.getPosition().y, L"Coordinate Y is not as expected");
			Assert::AreEqual(0.0, o.getPosition().theta, L"Coordinate THETA is not as expected");
		}

		bool compareDoubles(double d1, double d2, double delta = 0.5) {
			return (abs(d1 - d2) < abs(delta));
		}

		TEST_METHOD(TestTurningPositive) {
			//Counter clockwise
			double r = 1, d = 1, t2m = 100;
			Orientation o(d, r, 1/t2m);

			//To turn the robot, left needs to got backwards, and right forward
			//The distance for each wheel is 45deg on a unit circle
			double distMM = M_PI * d / 4;	//quarter of circle
			int ticksToTurn = (int)(distMM * t2m);
			double thetaFinal = 45;
			double thetaPerTick = thetaFinal / ticksToTurn;
			o.init();
			double thetaNow = 0.0;

			unsigned short ticksL = 0, ticksR = 0;

			for (int i = 0; i <= ticksToTurn; ++i, --ticksL, ++ticksR, thetaNow += thetaPerTick) {
				o.tick(ticksL, ticksR, thetaNow * 100);
			}

			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"X was not 0");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Y was not 0");
			Assert::IsTrue(compareDoubles(45, o.getPosition().theta), L"Theta was not 45");

		}

		TEST_METHOD(TestTurningNegative) {
			//clockwise
			double r = 1, d = 1, t2m = 100;
			Orientation o(d, r, 1 / t2m);

			//To turn the robot, right needs to got backwards, and left forward
			//The distance for each wheel is 45deg on a unit circle
			double distMM = M_PI * d / 4;	//quarter of circle
			int ticksToTurn = (int)(distMM * t2m);
			double thetaFinal = -45;
			double thetaPerTick = thetaFinal / ticksToTurn;
			o.init();
			double thetaNow = 0.0;

			unsigned short ticksL = 0, ticksR = 0;

			for (int i = 0; i <= ticksToTurn; ++i, ++ticksL, --ticksR, thetaNow += thetaPerTick) {
				o.tick(ticksL, ticksR, thetaNow * 100);
			}

			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"X was not 0");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Y was not 0");
			Assert::IsTrue(compareDoubles(-45, o.getPosition().theta), L"Theta was not -45");

		}


		TEST_METHOD(TestTurnAndForward) {
			//Counter clockwise
			double r = 1, d = 1, t2m = 100;
			Orientation o(d, r, 1 / t2m);

			//To turn the robot, left needs to got backwards, and right forward
			//The distance for each wheel is 45deg on a unit circle
			double distMM = M_PI * d / 2;	//quarter of circle
			int ticksToTurn = (int)(distMM * t2m);
			double thetaFinal = 90;
			double thetaPerTick = thetaFinal / ticksToTurn;
			o.init();
			double thetaNow = 0.0;

			unsigned short ticksL = 0, ticksR = 0;

			for (int i = 0; i <= ticksToTurn; ++i, --ticksL, ++ticksR, thetaNow += thetaPerTick) {
				o.tick(ticksL, ticksR, thetaNow * 100);
			}
			//turn done
			++ticksL; --ticksR; thetaNow -= thetaPerTick;

			//move forward, should increase Y
			double dist = 0.0;
			for (int i = 0; i < 500; ++i, dist += 1/t2m, ++ticksL, ++ticksR) {
				o.tick(ticksL, ticksR, thetaNow * 100);
			}

			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"X wasn't close to 0");
			Assert::IsTrue(compareDoubles(dist, o.getPosition().y), L"Y wasn't close to dist");
			Assert::IsTrue(compareDoubles(90, o.getPosition().theta), L"Theta wasn't close to 90");
		}
	};
}