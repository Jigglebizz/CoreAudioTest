#include "stdafx.h"
#include <winsdkver.h>
#include "CppUnitTest.h"

#include <objectModel/RenderingChain.h>

using namespace objectModel;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AudioProgramUnitTests
{
    // We need to insert a dummy class to test this functionality
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
            // Increment the buffer by 1 every time
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
            // GIVEN:...................................................................
            // Three Renderables
            RenderAccum* ra0 = new RenderAccum();
            RenderAccum* ra1 = new RenderAccum();
            RenderAccum* ra2 = new RenderAccum();
            // And a RenderingChain
            RenderingChain chain;

            // THEN:....................................................................
            // The chain is empty
            Assert::IsTrue(chain.size() == 0);

            // AND WHEN:................................................................
            // we insert we insert our first renderable
            chain.insert(0, ra0);

            // THEN:....................................................................
            // The chain has one element in it
            Assert::IsTrue(chain.size() == 1);
            // And the inserted Renderable is the same as the one we inserted
            Assert::IsTrue(dynamic_cast<RenderAccum*>(chain.get(0))->id == ra0->id);

            // AND WHEN:................................................................
            // We insert the second Renderable after the first
            chain.insert(1, ra1);

            // THEN:.....................................................................
            // The chain has two elements in it
            Assert::IsTrue(chain.size() == 2);
            // They are the same ones we inserted, in the positions they were inserted in
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(0))->id, ra0->id);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(1))->id, ra1->id);

            // AND WHEN:.................................................................
            // We insert the last one, in the same position as a previous element
            chain.insert(1, ra2);

            // THEN:.....................................................................
            // The chain has three elements in it
            Assert::IsTrue(chain.size() == 3);
            // All elements >= the insertion point are pushed back in the chain
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(0))->id, ra0->id);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(1))->id, ra2->id);
            Assert::AreEqual(dynamic_cast<RenderAccum*>(chain.get(2))->id, ra1->id);
        }

        TEST_METHOD(RenderingSequence) {
            // TODO: fill this out
        }
    };

    
}