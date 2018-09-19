#include <stdafx.h>
#include <winsdkver.h>
#include "CppUnitTest.h"

#include <objectModel/RenderingChain.cpp>
#include <objectModel/Renderable.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AudioProgramUnitTests
{
    class RenderAccum : public Renderable {
        static unsigned idAccum;
    public:
        const unsigned id;

        RenderAccum() :
            id(idAccum++)
        {}

        void render(float** Buffer,
            size_t NumChannels,
            size_t Length,
            const speakerService::Device& Dev) noexcept override
        {
            for (size_t c = 0; c < NumChannels; ++c) {
                for (size_t p = 0; p < Length; ++p) {
                    Buffer[c][p]++;
                }
            }
        }
    };

    unsigned RenderAccum::idAccum = 0;

    TEST_CLASS(RenderingChainTests)
    {
    public:
        TEST_METHOD(Insertion) {
            RenderAccum* ra0 = new RenderAccum();
            RenderAccum* ra1 = new RenderAccum();
            RenderAccum* ra2 = new RenderAccum();

            RenderingChain chain;

            Assert::IsTrue(chain.size() == 0);

            chain.insert(0, ra0);
            Assert::IsTrue(chain.size() == 1);
            Assert::IsTrue(dynamic_cast<RenderAccum*>(chain.get(0))->id == ra0->id);

            chain.insert(1, ra1);
            Assert::IsTrue(chain.size() == 2);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(0))->id, ra0->id);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(1))->id, ra1->id);

            chain.insert(1, ra2);
            Assert::IsTrue(chain.size() == 3);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(0))->id, ra0->id);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(1))->id, ra2->id);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(2))->id, ra1->id);
        }
    };

    
}