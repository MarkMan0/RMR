#include "pch.h"
#include "CppUnitTest.h"
#include "../RMR_Base/LoopRate.h"

#include "Helpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace LoopRateTests {

	TEST_CLASS(LoopRateTests) {
	public:
		using clock = std::chrono::high_resolution_clock;

		TEST_METHOD(TestCorrectWait) {
			LoopRate rate(1); //1Hz
			auto start = clock::now();
			rate.sleep();
			auto now = clock::now();

			std::chrono::duration<double> elapsed = now - start;
			Assert::IsTrue(compareDoubles(1.0, elapsed.count(), 0.005));

			start = clock::now();
			rate.sleep();
			now = clock::now(); 
			elapsed = now - start;
			Assert::IsTrue(compareDoubles(1.0, elapsed.count(), 0.005));
		}

		TEST_METHOD(TestWaitWhileBusy) {

			LoopRate rate(1);
			auto start = clock::now();
			std::this_thread::sleep_for(std::chrono::milliseconds(500));	//simualte task
			rate.sleep();
			auto after = clock::now();
			std::chrono::duration<double> elapsed = after - start;
			Assert::IsTrue(compareDoubles(1.0, elapsed.count(), 0.005));

			start = after;
			std::this_thread::sleep_for(std::chrono::milliseconds(800));	//second task
			rate.sleep();
			after = clock::now();
			elapsed = after - start;
			Assert::IsTrue(compareDoubles(1.0, elapsed.count(), 0.005));
		}
	
		
		TEST_METHOD(Test50Hz) {
			LoopRate rate(50);
			auto start = clock::now();
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			rate.sleep();
			auto after = clock::now();
			std::chrono::duration<double> elapsed = after - start;
			start = after;
			Assert::IsTrue(compareDoubles(1.0/50.0, elapsed.count(), 0.005));
			rate.sleep();
			after = clock::now();
			elapsed = after - start;
			Assert::IsTrue(compareDoubles(1.0 / 50.0, elapsed.count(), 0.005));
		}
	
	};
}