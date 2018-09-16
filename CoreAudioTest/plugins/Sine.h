#pragma once

#include "objectModel/Renderable.h"

namespace plugins {
    class Sine : public objectModel::Renderable {
    private:
        float mPitch;
        float mAngle;
    public:
        Sine();
        void render(    float** Buffer,
                        size_t NumChannels,
                        size_t Length,
                        const speakerService::Device& Dev) noexcept override;
    };
}