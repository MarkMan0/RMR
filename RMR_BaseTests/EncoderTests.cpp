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

			for (int i = start, dist = 0; i < max; ++i, ++dist) {
				enc.tick(i);
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
	};
}
