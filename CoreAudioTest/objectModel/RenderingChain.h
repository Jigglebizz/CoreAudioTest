#pragma once

#include "Renderable.h"

namespace objectModel {
   /**
    * Loaded into a device. A rendering chain is a collection of 
    * Renderables arranged in the order in which they will
    * operate on a buffer. 
    *
    * A RenderingChain is also a Renderable, meaning it's possible to 
    * nest RenderingChains within other RenderingChains
    *
    * EG:
    * [ Instrument] -> [ Effect] -> [ Effect]
    */
    class RenderingChain : Renderable {
    private:
        // Sometimes raw pointers are simpler to work with than smart pointers
        // As long as the domain is small enough, and ownership is clearly communicated
        // raw pointers can be much simpler to work with and maintain.
        using RenderList = std::vector< Renderable*>;
        RenderList mRenderables;
    public:
        ~RenderingChain() noexcept;

        size_t size() const noexcept;

        // Insert to rendering chain. Requires a heap-allocated object
        // The pointer is now owned and managed by the rendering chain
        void insert( size_t Position, Renderable* Render) noexcept;

        // Gets the pointer from the rendering chain
        Renderable* get(size_t Position);

        void render(  float** Buffer,
                      size_t NumChannels,
                      size_t Length,
                      const speakerService::Device& Dev) noexcept override;
    };
}
