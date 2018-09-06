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

        Device();
        Device(Device&& Other);
        Device( const std::wstring& Name,
                const std::wstring& Uid,
                IMMDevice *pDevice);

        Device(IMMDevice *pDevice);

        ~Device();
    };
}
