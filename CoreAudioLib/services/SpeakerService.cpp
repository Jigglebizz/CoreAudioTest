#include <stdafx.h>
#include "SpeakerService.h"

#include "core/WinDrivers.h"

using namespace speakerService;

SpeakerService::SpeakerService( Context& C)
    : mEnumerator( NULL),
      AbstractService( C)
{
    LPVOID pvReserved = NULL;
    HRESULT hr = S_OK;

    hr = CoInitializeEx(pvReserved, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
    if (FAILED(hr))
        throw std::runtime_error("Could not initialize Windows audio drivers!");

    hr = CoCreateInstance(
        WinDrivers::CLSID_MMDeviceEnumerator, NULL,
        CLSCTX_ALL, WinDrivers::IID_IMMDeviceEnumerator,
        (void**)&mEnumerator
    );
    if (FAILED(hr))
        throw std::runtime_error("Could not open audio device!");
}

SpeakerService::~SpeakerService() {
    SAFE_RELEASE(mEnumerator);
    CoUninitialize();
}

Device 
SpeakerService::GetDefaultDevice() const {
    IMMDevice *pDevice;

    HRESULT hr = mEnumerator->GetDefaultAudioEndpoint(
        eRender, eConsole, &pDevice
    );

    if (FAILED(hr))
        throw std::runtime_error("Could not get the default device!");

    auto dev = Device(pDevice);
    return dev;
}

std::vector< Device> 
SpeakerService::GetDevices() const {
    std::vector< Device> theDevices;

    // Boilerplate from Microsoft Docs:
    // https://docs.microsoft.com/en-us/windows/desktop/CoreAudio/device-properties
    // with some modern C++ stuff to make it a little nicer

    HRESULT hr = S_OK;
    IMMDeviceCollection *pCollection = NULL;
    IMMDevice *pEndpoint = NULL;

    try {
        hr = mEnumerator->EnumAudioEndpoints(
                            eRender, DEVICE_STATE_ACTIVE,
                            &pCollection );
        if (FAILED(hr))
            throw std::runtime_error("Could not enumerate audio endpoints!");

        UINT count;
        hr = pCollection->GetCount(&count);
        if (FAILED(hr))
            throw std::runtime_error("Could not get number of audio endpoints!");

        if (count == 0) {
            // Nothing found
            return {};
        }

        for (ULONG i = 0; i < count; ++i) {
            hr = pCollection->Item(i, &pEndpoint);
            if (FAILED(hr))
                throw std::runtime_error("Could not lookup device #" + std::to_string(i));

            theDevices.emplace_back( pEndpoint);
            pEndpoint = NULL;
        }
        SAFE_RELEASE(pCollection);

        return theDevices;
    }
    catch (const std::runtime_error&) {
        SAFE_RELEASE(pCollection);
        throw;
    }

    return{};
}