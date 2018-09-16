#include "stdafx.h"
#include "Sine.h"

#include "core/Math.h"

using namespace plugins;

Sine::Sine() :
    mPitch( 440.0f),
    mAngle( 0.0f)
{}

void 
Sine::render(   float ** Buffer, 
                size_t NumChannels, 
                size_t Length,
                const speakerService::Device& Dev) noexcept
{
    auto sr = Dev.getSampleRate();
    float angleDelta = (mPitch / sr) * 2 * math::pi;

    for (int f = 0; f < Length; ++f) {
        mAngle += angleDelta;
        for (int c = 0; c < NumChannels; ++c) {
            Buffer[c][f] = std::sin(mAngle);
        }
    }
}