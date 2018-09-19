#include <stdafx.h>
#include "RenderingChain.h"

using namespace objectModel;

RenderingChain::~RenderingChain() noexcept {
    for (auto& pointer : mRenderables) {
        delete pointer;
    }
}

size_t
RenderingChain::size() const noexcept {
    return mRenderables.size();
}

void
RenderingChain::insert(size_t Position, Renderable* Render) noexcept {

    size_t accum = 0;
    RenderList::iterator it;
    for (it = mRenderables.begin(); it != mRenderables.end(); ++it, ++accum) {
        if (accum == Position)
            break;
    }

    mRenderables.insert(it, Render);
}

Renderable*
RenderingChain::get(size_t Position) {
    if (Position > size())
        throw std::out_of_range("Attempt to access element out of range");
    
    size_t accum = 0;
    RenderList::iterator it;
    for (it = mRenderables.begin(); it != mRenderables.end(); ++it, ++accum) {
        if (accum == Position)
            break;
    }
   
    return *it;
}

void 
RenderingChain::render( float** Buffer,
                        size_t NumChannels,
                        size_t Length,
                        const speakerService::Device& Dev) noexcept 
{
    for(auto& renderable : mRenderables) {
        renderable->render(Buffer, NumChannels, Length, Dev);
    }
}