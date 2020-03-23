#include "pch.h"
#include "CppUnitTest.h"

#include "../RMR_Base/Helpers.h"
#include "Helpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace ControllerTests {

	TEST_CLASS(ControllerTests) {
	public:

		TEST_METHOD(TestFindClosestAngle) {
			double curr, target;

			curr = 10;
			target = 20;
			Assert::IsTrue(compareDoubles(20, getClosestTargetAngle(curr, target)), L"Failed for [10, 20]");

			curr = 10;
			target = -10;
			Assert::IsTrue(compareDoubles(-10, getClosestTargetAngle(curr, target)), L"Failed for [10, -10]");

			curr = 170;
			target = -170;
			Assert::IsTrue(compareDoubles(190, getClosestTargetAngle(curr, target)), L"Failed for [170, -170]");

			curr = -180;
			target = 180;
			Assert::IsTrue(compareDoubles(-180, getClosestTargetAngle(curr, target)), L"Failed for [180, -180]");

			curr = -170;
			target = 170;
			Assert::IsTrue(compareDoubles(-190, getClosestTargetAngle(curr, target)), L"Failed for [-170, 170]");


		}
	};
}