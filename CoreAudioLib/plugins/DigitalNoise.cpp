#include "stdafx.h"
#include "DigitalNoise.h"

using namespace plugins;

void 
DigitalNoise::render(   float ** Buffer, 
                        size_t NumChannels, 
                        size_t Length, 
                        const speakerService::Device & Dev) noexcept
{
    float sample;
    for (int f = 0; f < Length; ++f) {
        sample = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        for (int c = 0; c < NumChannels; ++c) {
            Buffer[c][f] = sample;
        }
    }
}
