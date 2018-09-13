#include <stdafx.h>
#include "AudioBuffer.h"

AudioBuffer::AudioBuffer( size_t Size) {
    mBufImpl.reserve(Size);
}

void
AudioBuffer::addSample( const double& value) {
    if (mBufImpl.size() != mBufImpl.capacity())
        throw std::overflow_error("Buffer overrun");

    mBufImpl.push_back(value);
}

void
AudioBuffer::getBuffer(BYTE** Buf) const noexcept {
    for ( int i = 0; i < mBufImpl.capacity(); ++i) {
        (*Buf)[i] = (i < mBufImpl.size()) ? 
            static_cast<BYTE>(mBufImpl[i] * 127.0 + 127.0) : 
            0;
    }
}