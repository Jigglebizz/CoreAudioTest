#pragma once

class AudioBuffer {
private:
    std::vector<double> mBufImpl;
public:
    AudioBuffer( size_t Size);

    size_t size() const noexcept;
    void pokeSample(size_t Index, double Value);
    double peekSample( size_t Index) const;

    // This template allows you get the audio out
    // in whatever unsigned integer precision you would like.
    // This is useful for supporting higher or lower bit-depth
    // audio DACs
    template< typename T>
    void getBuffer(T** Buf) const noexcept {
        double halfMax = static_cast<double>(std::numeric_limits<T>::max() / 2);

        for (int i = 0; i < mBufImpl.capacity(); ++i) {
            (*Buf)[i] = (i < mBufImpl.size()) ?
                static_cast<T>(mBufImpl[i] * halfMax + halfMax) :
                0;
        }
    }
};