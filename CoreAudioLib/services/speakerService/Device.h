#pragma once

#include "core/WinDrivers.h"
#include "core/AudioBuffer.h"

namespace objectModel {
    class RenderingChain;
}

namespace speakerService {
    // The device renders audio to an endpoint.
    // Open or close the device to begin/end rendering.
    // Get the rendering chain to provide programs to render.
    class Device {
    private:
    testable_public
        static uint64_t idCounter;
        static constexpr uint32_t cDefaultBufferSize = 1024;
        static constexpr uint32_t cDefaultSampleRate = 44'100;
        static constexpr uint32_t cDefaultBitsPerSample = 8;
        static constexpr uint32_t cDefaultBytesPerFrame = 1;
        static const std::chrono::milliseconds cDefaultLatency;

        std::wstring mName;
        std::wstring mUid;
        uint64_t mAppId;
        uint32_t mBufferSize;
        uint32_t mSampleRate;
        uint32_t mBitsPerSample;
        uint32_t mBytesPerFrame;
        unsigned mNumChannels;
        IMMDevice* mWinDevice;
        IAudioRenderClient *mRenderClient;
        IAudioClient *mAudioClient;

        std::chrono::milliseconds mLatency;

        bool mOpened;
        std::unique_ptr<objectModel::RenderingChain> mRenderingChain;

        std::thread mRenderingThread;
        std::atomic_bool mCloseRequested;
        std::atomic_bool mRenderThreadClosed;
        void render() noexcept;
        inline void floatToWinBuf( float** FloatBuf, BYTE* WinBuf, size_t NumFrames) const noexcept;
    public:
        Device() noexcept;
        Device(IMMDevice *pDevice) noexcept;

        Device( Device& Other) noexcept;
        Device& operator=(Device Other) noexcept;
        Device(Device&& Other) noexcept;

        friend void swap(Device& A, Device& B) noexcept;

        ~Device();

        const std::wstring getName() const noexcept { 
            return mName;  
        }
        const std::wstring getUid() const noexcept { 
            return mUid;  
        }
        const uint64_t getAppId() const noexcept { 
            return mAppId; 
        }
        const uint32_t getBufferSize() const noexcept { 
            assert (mOpened && 
                "Cannot get buffer size of device until it is opened"); 
            return mBufferSize;  
        }
        const uint32_t getSampleRate() const noexcept { 
            assert(mOpened && 
                "Cannot get sample rate of device until it is opened");
            return mSampleRate;
        }
        const uint32_t getBitDepth() const noexcept {
            assert(mOpened &&
                "Cannot get bit depth of device until it is opened");
            return mBitsPerSample;
        }
        const unsigned getNumChannels() const noexcept {
            assert(mOpened &&
                "Cannot get number of channels of a device until it is opened");
            return mNumChannels;
        }
        const std::chrono::milliseconds getLatency() const noexcept {
            return mLatency;
        }
        void setLatency( const std::chrono::milliseconds& latency) noexcept {
            mLatency = latency;
        }

        objectModel::RenderingChain& getChain() noexcept;

        void open();
        void close();
    };
}
