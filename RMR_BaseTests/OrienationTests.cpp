#include "pch.h"
#include "CppUnitTest.h"

#include "../RMR_Base/Orientation.h"
#include "../RMR_Base/Orientation.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace OrientationTests {

	TEST_CLASS(OrientationTests) {
	private:
		const double mmPerTickKobuki = 0.000085292090497737556558 * 1000.0,
			dKobuki = 230, rKobuki = 35;

		bool compareDoubles(double d1, double d2, double delta = 0.01) {
			return (abs(d1 - d2) < abs(delta));
		}

		double deg2rad(double deg) {
			return (deg / 360 * 2 * M_PI);
		}
		double rad2deg(double rad) {
			return (rad / 2 / M_PI * 360);
		}

		void moveLinear(Orientation& o, int ticks, unsigned short& ticksL, unsigned short& ticksR, signed short& angleTicks) {
			int dir = ticks > 0 ? 1 : -1;
			ticks = abs(ticks);
			for (int i = 0; i < ticks; ++i) {
				//move 100 ticks with both wheels
				ticksL += dir;
				ticksR += dir;
				o.tick(ticksL, ticksR, angleTicks);
			}
		}

		void moveRotate(Orientation& o, double angle, double d, double r, double mmPerTick,
					unsigned short& ticksL, unsigned short& ticksR, signed short& angleTicks) {

			double distMM = d/2.0 *  deg2rad(angle);	// each wheel has to move this distance
			int ticksToTurn = abs((int)(distMM / mmPerTick));	//the required ticks to move the distance

			if (abs((ticksToTurn + 1.0) * mmPerTick - distMM) < abs((ticksToTurn)*mmPerTick - distMM)
				&& abs((ticksToTurn + 1.0) * mmPerTick - distMM) < abs((ticksToTurn - 1.0) * mmPerTick - distMM)) {
				++ticksToTurn;
			}
			else if(abs((ticksToTurn - 1.0) * mmPerTick - distMM) < abs((ticksToTurn)*mmPerTick - distMM)
				&& abs((ticksToTurn - 1.0) * mmPerTick - distMM) < abs((ticksToTurn + 1.0) * mmPerTick - distMM)) {
				--ticksToTurn;
			}
			
			double thetaPerTick = 100 * angle / ticksToTurn;

			

			int dir = angle > 0 ? 1 : -1;
			double tickBuff = angleTicks;
			for (int i = 0; i < abs(ticksToTurn); ++i) {
				ticksL -= dir;
				ticksR += dir;
				tickBuff += thetaPerTick;		//to eliminate orunding errors over time
				angleTicks = (signed short) round(tickBuff);
				o.tick(ticksL, ticksR, angleTicks);
			}
		}



	public:

		TEST_METHOD(TestTranslation) {
			double r = rKobuki, d = dKobuki, mmPerTick = mmPerTickKobuki;
			Orientation o(d, r, mmPerTick);

			o.init();

			unsigned short xTick = 0, yTick = 0;
			signed short angleTicks = 0;

			moveLinear(o, 500, xTick, yTick, angleTicks);

			Assert::IsTrue(compareDoubles(mmPerTick * 500, o.getPosition().x),
				L"FWD: Coordinate X is not as expected");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), 
				L"FWD: Coordinate Y is not as expected");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().theta), 
				L"FWD: Coordinate THETA is not as expected");


			moveLinear(o, -1000, xTick, yTick, angleTicks);

			Assert::IsTrue(compareDoubles(-500 * mmPerTick, o.getPosition().x),
				L"REV: Reverse: Coordinate X is not as expected");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y),
				L"REV: Coordinate Y is not as expected");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().theta),
				L"REV: Coordinate THETA is not as expected");
		}



		TEST_METHOD(TestRotation) {
			//Counter clockwise
			double r = rKobuki, d = dKobuki, mmPerTick = mmPerTickKobuki;
			Orientation o(d, r, mmPerTick);
			o.init();
			unsigned short xTicks = 0, yTicks = 0;
			signed short angleTicks = 0;
			//To turn the robot, left needs to got backwards, and right forward
			//The distance for each wheel is 45deg on a unit circle
			moveRotate(o, 45, d, r, mmPerTick, xTicks, yTicks, angleTicks);	//positive

			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"X was not 0");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Y was not 0");
			Assert::IsTrue(compareDoubles(45, o.getPosition().theta), L"Theta was not 45");


			moveRotate(o, -90, d, r, mmPerTick, xTicks, yTicks, angleTicks); //negative
			
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"X was not 0");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Y was not 0");
			Assert::IsTrue(compareDoubles(-45, o.getPosition().theta, 1.0), L"Theta was not -45");
		}




		TEST_METHOD(TestTurnAndForward) {
			//Counter clockwise
			double r = rKobuki, d = dKobuki, mmPerTick = mmPerTickKobuki;
			Orientation o(d, r, mmPerTick);
			unsigned short xTicks = 0, yTicks = 0;
			signed short angleTicks = 0;

			o.init();

			// X-direction+
			moveLinear(o, 500, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().x), L"Move 1: X");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Move 1: Y");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().theta), L"Move 1: Theta");

			//turn to Y+
			moveRotate(o, 90, d, r, mmPerTick, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().x), L"Rotate 1: X");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Rotate 1: Y");
			Assert::IsTrue(compareDoubles(90, o.getPosition().theta), L"Rotate 1: Theta");

			// move along Y+
			moveLinear(o, 500, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().x), L"Move 2: X");
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().y), L"Move 2: Y");
			Assert::IsTrue(compareDoubles(90, o.getPosition().theta), L"Move 2: Theta");

			//turn to X-
			moveRotate(o, 90, d, r, mmPerTick, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().x), L"Rotate 2: X");
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().y), L"Rotate 2: Y");
			Assert::IsTrue(compareDoubles(180, o.getPosition().theta), L"Rotate 2: Theta");

			//move along X-
			moveLinear(o, 500, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"Move 3: X");
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().y), L"Move 3: Y");
			Assert::IsTrue(compareDoubles(180, o.getPosition().theta), L"Move 3: Theta");

			//turn to Y-
			moveRotate(o, 90, d, r, mmPerTick, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"Rotate 3: X");
			Assert::IsTrue(compareDoubles(500 * mmPerTick, o.getPosition().y), L"Rotate 3: Y");
			Assert::IsTrue(compareDoubles(270, o.getPosition().theta), L"Rotate 3: Theta");

			//move along Y-
			moveLinear(o, 500, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"Move 4: X");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Move 4: Y");
			Assert::IsTrue(compareDoubles(270, o.getPosition().theta), L"Move 4: Theta");

			//turn to X+
			moveRotate(o, 90, d, r, mmPerTick, xTicks, yTicks, angleTicks);
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().x), L"Rotate 4: X");
			Assert::IsTrue(compareDoubles(0.0, o.getPosition().y), L"Rotate 4: Y");
			Assert::IsTrue(compareDoubles(360, o.getPosition().theta), L"Rotate 4: Theta");

		}
	};
}