#include <winsdkver.h>
#include "CppUnitTest.h"

#include <core/AudioBuffer.cpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AudioProgramUnitTests
{		
	TEST_CLASS(AudioBufferTests)
	{
	public:
		
		TEST_METHOD(InitializeEmpty) {
            AudioBuffer buf(256);
            BYTE* outputBuf = (BYTE*)malloc(256);
            buf.getBuffer<BYTE>(&outputBuf);

            for (size_t i = 0; i < 256; ++i) {
                Assert::AreEqual(outputBuf[i], static_cast<BYTE>(127));
            }
            free(outputBuf);
		}
        TEST_METHOD(ArbitraryPrecision) {
            AudioBuffer buf(32);
            uint32_t* outputBuf = (uint32_t*)malloc(32 * sizeof(uint32_t));
            buf.getBuffer<uint32_t>(&outputBuf);

            for (size_t i = 0; i < 32; ++i) {
                Assert::AreEqual(outputBuf[i], static_cast<uint32_t>(2147483647));
            }
        }

	};
}