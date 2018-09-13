#pragma once

class AudioBuffer {
private:
    std::vector<double> mBufImpl;
public:
    AudioBuffer( size_t Size);
    
    void addSample( const double& value);

    void getBuffer(Byte** Buf) const noexcept;
};