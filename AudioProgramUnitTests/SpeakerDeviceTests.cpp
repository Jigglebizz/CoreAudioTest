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

            // GIVEN:...............................................
            // a device
            speakerService::Device d;

            // A buffer of uint32_t's, expressed as BYTEs
            BYTE* winBuf = new BYTE[ frames * sizeof(uint32_t)]();

            //v. an array of an array of ~~floats
            //    (channels)  (frames)   (samples)
            float** floatBuf = new float*[1]();
            floatBuf[0] = new float[frames]();
            //^.

            // that has been filled with max audio values
            for (unsigned i = 0; i < frames; ++i) {
                floatBuf[0][i] = 1.0f;
            }
            
            // WHEN:.................................................
            // The float arrays are converted to Microsoft's
            // preferred PWM format
            //d.floatToWinBuf( floatBuf, winBuf, 128);

            // THEN:.................................................
            // The buffer of BYTE's is filled with all max
            // PWM values
            for (unsigned i = 0; i < 128; ++i) {
                Assert::IsTrue(
                    (uint32_t&)winBuf[i * sizeof(uint32_t)] == 
                    std::numeric_limits<uint32_t>::max()
                );
            }
        }
    };
}

// [1.]
// I really like constexpr. I've been interested in exploring
// constexpr template functions for developing a
// meta-language, but I haven't found an opportunity where
// it would be practical yet...