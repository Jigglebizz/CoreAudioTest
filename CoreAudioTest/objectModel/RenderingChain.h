#pragma once

#include "Renderable.h"

namespace objectModel {
   /**
    * Loaded into a device. A rendering chain is a collection of 
    * Renderables arranged in the order in which they will
    * operate on a buffer
    *
    * EG:
    * [ Instrument] -> [ Effect] -> [ Effect]
    */
    class RenderingChain : Renderable {
    private:
        using RenderList = std::vector< std::unique_ptr<Renderable>>;
        RenderList mRenderables;
    public:
        size_t size() const noexcept;
        void insert( size_t Position, Renderable* XRender) noexcept;
        Renderable* get(size_t Position);

        void render(  float** Buffer,
                      size_t NumChannels,
                      size_t Length) noexcept override;
    };
}
