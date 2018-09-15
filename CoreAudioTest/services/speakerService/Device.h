#pragma once

#include "core/WinDrivers.h"
#include "core/AudioBuffer.h"

namespace speakerService {
    // The device is used to stream to an audio endpoint
    // It has a simple interface but it must be used in 
    // a specific way
    class Device {
    private:
        static uint64_t idCounter;
        static constexpr uint32_t cDefaultBufferSize = 1024;
        static constexpr uint32_t cDefaultSampleRate = 44'100;
        static constexpr uint32_t cDefaultBitsPerSample = 8;
        static const std::chrono::milliseconds cDefaultLatency;

        std::wstring mName;
        std::wstring mUid;
        uint64_t mAppId;
        uint32_t mBufferSize;
        uint32_t mSampleRate;
        uint32_t mBitsPerSample;
        unsigned mNumChannels;
        IMMDevice* mWinDevice;
        IAudioRenderClient *mRenderClient;
        IAudioClient *mAudioClient;

        std::chrono::milliseconds mLatency;

        bool mOpened;

    public:
        Device() noexcept;
        Device(Device&& Other) noexcept;
        Device(const Device& Other) = delete; // It is wise to not copy winDevice

        Device(IMMDevice *pDevice) noexcept;

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

        void Open();
        void Close();
        void Render( const AudioBuffer& Buf);

        void PlayTestTone(const std::chrono::nanoseconds Duration);
    };
}
