#pragma once

#include <string>
#include <memory>

#include "core/WinDrivers.h"
#include "core/AudioBuffer.h"

namespace speakerService {
    class Device {
    private:
        static uint64_t idCounter;
        static constexpr uint32_t cDefaultBufferSize = 1024;
        static constexpr uint32_t cDefaultSampleRate = 44'100;

        std::wstring mName;
        std::wstring mUid;
        uint64_t mAppId;
        uint32_t mBufferSize;
        uint32_t mSampleRate;
        IMMDevice* mWinDevice;

    public:
        Device() noexcept;
        Device(Device&& Other) noexcept;
        Device(const Device& Other) = delete; // It is wise to not copy winDevice

        Device(IMMDevice *pDevice) noexcept;

        ~Device();

        const std::wstring getName() const noexcept { return mName;  }
        const std::wstring getUid() const noexcept { return mUid;  }
        const uint64_t getAppId() const noexcept { return mAppId; }
        const uint32_t getBufferSize() const noexcept { return mBufferSize;  }
        const uint32_t getSampleRate() const noexcept { return mSampleRate;  }

        void Render( const AudioBuffer<uint32_t>& Buf);
    };
}
