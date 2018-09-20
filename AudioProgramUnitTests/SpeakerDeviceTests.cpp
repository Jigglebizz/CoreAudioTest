#include <stdafx.h>

#include <winsdkver.h>
#include "CppUnitTest.h"

#include <Windows.h>

#include "services/speakerService/Device.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AudioProgramUnitTests
{
    TEST_CLASS(SpeakerDeviceTests)
    {
    public:
        TEST_METHOD(ConvertToWinBuf) {
            constexpr size_t frames = 128; // [1.]
            constexpr size_t channels = 1;

            // GIVEN:...............................................
            // a device
            speakerService::Device d;
            d.mBytesPerFrame = 4;

            // A buffer of uint32_t's
            uint32_t* winBuf = new uint32_t[ frames ]();

            //v. an array of an array of ~~floats
            float** floatBuf = new float*[channels]();
            floatBuf[0] = new float[frames]();
            //^.

            // that has been filled with max audio values
            for (unsigned i = 0; i < frames; ++i) {
                floatBuf[0][i] = 1.0f;
            }
            
            // WHEN:.................................................
            // The float arrays are converted to Microsoft's
            // preferred PWM format
            d.floatToWinBuf( floatBuf, reinterpret_cast<BYTE*>(winBuf), frames);

            // THEN:.................................................
            // The buffer of uint32_t's is filled with all max
            // PWM values
            uint32_t maxVal = std::numeric_limits<uint32_t>::max();
            for (unsigned i = 0; i < frames; ++i) {
                Assert::IsTrue( winBuf[i] == std::numeric_limits<uint32_t>::max() - 1 );
            }
        }
    };
}

// [1.]
// I really like constexpr. I've been interested in exploring
// constexpr template functions for developing a
// meta-language, but I haven't found an opportunity where
// it would be practical yet...