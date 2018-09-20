#include "stdafx.h"

#include <winsdkver.h>
#include "CppUnitTest.h"

#include <core/AudioBuffer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AudioProgramUnitTests
{		
    TEST_CLASS(AudioBufferTests)
	{
	public:
		TEST_METHOD(InitializeEmpty) {
            constexpr size_t frames = 256;
            constexpr BYTE halfByteMax = std::numeric_limits<BYTE>::max() / 2;

            // GIVEN:......................................................
            // An AudioBuffer object, initialized to all zeros
            AudioBuffer buf(frames);
            // And a Windows PWM buffer of the same size
            BYTE* outputBuf = new BYTE[frames]();

            // WHEN:.......................................................
            // We ask the AudioBuffer to convert its data to PWM
            buf.getBuffer<BYTE>(&outputBuf);

            // THEN:.......................................................
            // The PWM Buffer is filled with samples
            // at the speaker diaphragm's 0-deflection point
            for (size_t i = 0; i < frames; ++i) {
                Assert::AreEqual(outputBuf[i], static_cast<BYTE>(halfByteMax));
            }

            //cleanup:
            free(outputBuf);
        }
        TEST_METHOD(ArbitraryPrecision) {
            constexpr size_t frames = 32;
            constexpr uint32_t halfUint32Max = std::numeric_limits<uint32_t>::max() / 2;

            // GIVEN:........................................................
            // An audio buffer object, initialized to all zeros
            AudioBuffer buf(frames);
            // And a uint32_t PWM buffer of the same size
            uint32_t* outputBuf = new uint32_t[frames]();

            // WHEN:.........................................................
            // The audio buffer poops out PWM
            buf.getBuffer<uint32_t>(&outputBuf);

            // THEN:.........................................................
            // The PWM Buffer is filled with samples
            // at the speaker diaphragm's 0-deflection point
            for (size_t i = 0; i < frames; ++i) {
                Assert::AreEqual(outputBuf[i], static_cast<uint32_t>(halfUint32Max));
            }

            // cleanup
            free(outputBuf);
        }
	};
}