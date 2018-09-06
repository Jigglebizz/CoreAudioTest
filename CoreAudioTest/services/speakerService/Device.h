#pragma once

#include <string>
#include <memory>

#include "WinDriverUtils/common.h"

namespace speakerService {
    struct Device {
        static uint64_t idCounter;

        std::wstring name;
        std::wstring uid;
        uint64_t appId;
        IMMDevice* winDevice;

        Device() noexcept;
        Device(Device&& Other) noexcept;
        Device(const Device& Other) = delete; // It is wise not to copy winDevice

        Device( const std::wstring& Name,
                const std::wstring& Uid,
                IMMDevice *pDevice) noexcept;

        Device(IMMDevice *pDevice) noexcept;

        ~Device();
    };
}
