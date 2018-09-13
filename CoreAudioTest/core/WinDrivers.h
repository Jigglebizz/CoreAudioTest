#pragma once

#define SAFE_RELEASE(punk) \
                if ((punk) != NULL) \
                  { (punk)->Release(); (punk) = NULL; }

struct WinDrivers {
    static const CLSID CLSID_MMDeviceEnumerator;
    static const IID IID_IMMDeviceEnumerator;
    static const IID IID_IAudioClient;
    static const IID IID_IAudioRenderClient;

    static const unsigned REFTIMES_PER_SEC;
    static const unsigned REFTIMES_PER_MS;
};
