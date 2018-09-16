#include <stdafx.h>
#include "WinDrivers.h"

const CLSID WinDrivers::CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID WinDrivers::IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID WinDrivers::IID_IAudioClient = __uuidof(IAudioClient);
const IID WinDrivers::IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

const unsigned WinDrivers::REFTIMES_PER_SEC = 10'000'000;
const unsigned WinDrivers::REFTIMES_PER_MS = 10'000;

std::string
WinDrivers::audioClientErrorString( HRESULT hr) {
    switch (hr) {
    case AUDCLNT_E_BUFFER_ERROR:
        return "buffer error";
    case AUDCLNT_E_BUFFER_TOO_LARGE:
        return "buffer too large";
    case AUDCLNT_E_INVALID_SIZE:
        return "invalid size"; 
    case AUDCLNT_E_BUFFER_SIZE_ERROR:
        return "buffer size error";
    case AUDCLNT_E_OUT_OF_ORDER:
        return "out of order";
    case AUDCLNT_E_DEVICE_INVALIDATED:
        return "device invalidated";
    case AUDCLNT_E_BUFFER_OPERATION_PENDING:
        return "operation pending";
    case AUDCLNT_E_SERVICE_NOT_RUNNING:
        return "service not running";
    case E_POINTER:
        return "bad pointer";
    case E_INVALIDARG:
        return "invalid argument";
    }
    return "Unknown Error";
}