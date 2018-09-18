#include <winsdkver.h>
#include "CppUnitTest.h"
#define NOMINMAX
#include <Windows.h>

#include "stdafx.h"
#include "services/speakerService/Device.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AudioProgramUnitTests
{
    TEST_CLASS(SpeakerDeviceTests)
    {
    public:
        TEST_METHOD(ConvertToWinBuf) {
            speakerService::Device d;

            BYTE* winBuf = new BYTE[ 128 * sizeof(uint32_t)]();
            float** floatBuf = new float*[1]();
            floatBuf[0] = new float[128]();

            for (unsigned i = 0; i < 128; ++i) {
                floatBuf[0][i] = 1.0f;
            }
            
            d.floatToWinBuf( floatBuf, winBuf, 128);

            for (unsigned i = 0; i < 128; ++i) {
                Assert::IsTrue(
                    (uint32_t&)winBuf[i * sizeof(uint32_t)] == 
                    std::numeric_limits<uint32_t>::max()
                );
            }
        }
    };
}