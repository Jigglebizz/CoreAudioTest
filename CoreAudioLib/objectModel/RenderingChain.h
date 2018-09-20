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
        using RenderList = std::vector< Renderable*>;   // [1.]
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

// [1.]
// Sometimes raw pointers are simpler to work with than smart pointers
// As long as the domain is small enough, and ownership is clearly communicated
// raw pointers can be much simpler to work with and maintain.
//
// For example, you don't need to worry about copying an array of raw pointers
// like you do an array of unique_ptr
//
// Smart pointers are designed to do two things:
// 1) Make using pointers easier for folks coming from languages without pointers
// 2) Express ownership
//
// Well, I could make an array of weak_ptr, or shared_ptr, but my ownership
// gets obscured that way. I would rather use a raw pointer, which will cause
// the maintainer to pay extra attention and be deliberate about
// ownership. Or introduce bugs. But hopefully they would run `git blame`
// and come talk to me before doing that.