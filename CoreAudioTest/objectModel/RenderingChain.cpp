#include <stdafx.h>
#include "RenderingChain.h"

using namespace objectModel;

size_t
RenderingChain::size() const noexcept {
    return mRenderables.size();
}

void
RenderingChain::insert(size_t Position, Renderable* XRender) noexcept {

    size_t accum = 0;
    RenderList::iterator it;
    for (it = mRenderables.begin(); it != mRenderables.end(); ++it, ++accum) {
        if (accum == Position)
            break;
    }

    mRenderables.emplace(it, XRender);
}

Renderable*
RenderingChain::get(size_t Position) {
    if (Position > size())
        throw std::out_of_range("Attempt to access element out of range");

    return mRenderables.at(Position).get();
}

void 
RenderingChain::render( float** Buffer,
                        size_t NumChannels,
                        size_t Length) noexcept 
{
    for(auto& renderable : mRenderables) {
        renderable->render(Buffer, NumChannels, Length);
    }
}