#pragma once

#include "objectModel/Renderable.h"

namespace plugins {
    class DigitalNoise : public objectModel::Renderable {
    public:
        void render(float** Buffer,
            size_t NumChannels,
            size_t Length,
            const speakerService::Device& Dev) noexcept override;
    };
}

