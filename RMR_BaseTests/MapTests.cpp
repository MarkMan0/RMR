#include "pch.h"
#include "CppUnitTest.h"

#include "../RMR_Base/Helpers.h"
#include "Helpers.h"
#include <mutex>

#define TESTING
#define private public
#include "../RMR_Base/LidarData.h"
#include "../RMR_Base/LidarData.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace MapTests {

	TEST_CLASS(MapTests) {

	public:

		TEST_METHOD(testGetScaledInd) {
			const int spacing = 100;
			lidar::Map map(spacing);

			Assert::AreEqual(0, map.getClosestCoord(0));
			Assert::AreEqual(0, map.getClosestCoord(spacing / 2));
			Assert::AreEqual(0, map.getClosestCoord(spacing - 1));
			Assert::AreEqual(spacing, map.getClosestCoord(spacing));
			Assert::AreEqual(spacing, map.getClosestCoord(1.5 * spacing));
			Assert::AreEqual(-spacing, map.getClosestCoord(-1.5 * spacing));

			Assert::AreEqual(2 * spacing, map.getClosestCoord(2 * spacing));
		}

	};

}