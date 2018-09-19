#include <stdafx.h>
#include "AudioBuffer.h"

AudioBuffer::AudioBuffer( size_t Size)
    : mBufImpl( Size, 0)
{}

size_t
AudioBuffer::size() const noexcept {
    return mBufImpl.size();
}

double
AudioBuffer::peekSample( size_t Index) const {
    if (Index >= mBufImpl.capacity())
        throw std::out_of_range("Index out of buffer range");

    return mBufImpl.at(Index);
}

void
AudioBuffer::pokeSample( size_t Index, double Value) {
    if (Index >= mBufImpl.capacity())
        throw std::out_of_range("Index out of buffer range");

    mBufImpl[Index] = Value;
}