#include "pch.h"
#include "CppUnitTest.h"

#include "../RMR_Base/Encoder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EncoderTests
{
	TEST_CLASS(EncoderTests)
	{
	public:
		unsigned short max = std::numeric_limits<unsigned short>::max();
		unsigned short min = std::numeric_limits<unsigned short>::min();

		TEST_METHOD(TestNormalBehaviour)
		{
			Encoder<unsigned short, double> enc(1);
			unsigned short start = min;
			enc.begin(start); //encoder just started near max
			unsigned short tick = start;
			for (int i = start, dist = 0; i < max; ++i, ++dist, ++tick) {
				enc.tick(tick);
				Assert::AreEqual(1.0 * dist, enc.getPosition());
			}

			// Normal range [min, max] passed

		}

		TEST_METHOD(TestOverflow) {
			Encoder<unsigned short, double> enc(1);
			unsigned short start = max - 100;
			enc.begin(start);
			int dist = 0;
			unsigned short tick = start;
			for (int i = 0; i < 500; ++i, ++dist, ++tick) {
				enc.tick(tick);
				Assert::AreEqual(1.0 * dist, enc.getPosition());
			}
		}


		TEST_METHOD(TestUnderflow) {
			Encoder<unsigned short, double> enc(1);
			unsigned short start = min + 100;
			enc.begin(start);
			int dist = 0;
			unsigned short tick = start;
			for (int i = 0; i < 500; ++i, --dist, --tick) {
				enc.tick(tick);
				Assert::AreEqual(1.0 * dist, enc.getPosition());
			}
		}

		TEST_METHOD(TestExtended) {
			return;
			Encoder<unsigned short, double> enc(1);
			unsigned short start = (max - min) / 2;
			long dist = 0;
			enc.begin(start);
			for (long i = 0; i < 500000; ++i, ++start, ++dist) {
				enc.tick(start);
				Assert::AreEqual(1.0 * dist, enc.getPosition());
			}
			for (long i = 0; i < 500000; ++i, --start, --dist) {
				enc.tick(start);
				Assert::AreEqual(1.0 * dist, enc.getPosition());
			}

		}

		TEST_METHOD(TestForAngle) {
			Encoder<signed short, double> enc(1);
			auto min = std::numeric_limits<signed short>::min();
			auto max = std::numeric_limits<signed short>::max();
			signed short start = min;
			enc.begin(start); //encoder just started near max
			signed short tick = start;
			int dist = 0;
			for (int i = start; i < max; ++i, ++dist, ++tick) {
				enc.tick(tick);
				Assert::AreEqual(1.0 * dist, enc.getPosition(), L"Normal behaviour failed for Angle");
			}

			//Test overflow

			start = max - 100;
			tick = start;
			dist = 0;
			enc.begin(start); //reset encoder

			for (int i = 0; i < 1000; ++i, ++start, ++dist, ++tick) {
				enc.tick(tick);
				Assert::AreEqual(1.0 * dist, enc.getPosition(), L"Overflow behaviour failed for Angle");
			}


			//Test underflow
			start = min + 100;
			tick = start;
			dist = 0;
			enc.begin(start); //reset encoder

			for (int i = 0; i < 1000; ++i, --dist, --tick) {
				enc.tick(tick);
				Assert::AreEqual(1.0 * dist, enc.getPosition(), L"Underflow behaviour failed for Angle");
			}
		}

	};
}
