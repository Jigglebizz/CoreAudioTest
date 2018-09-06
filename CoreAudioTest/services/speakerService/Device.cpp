#include <stdafx.h>
#include "Device.h"

using namespace speakerService;

uint64_t Device::idCounter = 0;

Device::Device()
    : name(L"Unknown"), winDevice(NULL)
{}

Device::Device( const std::wstring& Name,
                const std::wstring& Uid,
                IMMDevice *pDevice )
    : name(Name), uid(Uid), winDevice(pDevice)
{}

Device::Device( Device&& Other) :
    name( Other.name), 
    uid( Other.uid),
    appId( Other.appId),
    winDevice( Other.winDevice)
{
    Other.winDevice = NULL;
    Other.name = L"Unknown";
    Other.appId = std::numeric_limits<uint64_t>::max();
    Other.uid = L"";
}

Device::Device(IMMDevice *pDevice) 
  : name(L"Unknown"), winDevice(NULL)
{
    HRESULT hr = S_OK;
    LPWSTR pwszID = NULL;
    IPropertyStore *pProps = NULL;

    try {
        hr = pDevice->GetId(&pwszID);
        if (FAILED(hr))
            throw std::runtime_error("Could not get device ID of " +
            (appId == -1) ? "default device" : "device #" + std::to_string(appId));

        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (FAILED(hr))
            throw std::runtime_error("Could not open property store for " +
            (idCounter == -1) ? "default device" : "device #" + std::to_string(appId));

        PROPVARIANT varName;
        PropVariantInit(&varName);
        hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
        if (FAILED(hr))
            throw std::runtime_error("Failed to get property 'Friendly Name' for " +
            (appId == -1) ? "default device" : "device #" + std::to_string(appId));

        name = varName.pwszVal;
        uid = pwszID;
        winDevice = pDevice;

        CoTaskMemFree(pwszID);
        pwszID = NULL;
        PropVariantClear(&varName);
        SAFE_RELEASE(pProps);
    }
    catch (std::runtime_error&) {
        CoTaskMemFree(pwszID);
        pwszID = NULL;
        SAFE_RELEASE(pProps);
    }

    appId = idCounter++;
}

Device::~Device() {
    if (winDevice != NULL) {
        winDevice->Release();
    }
}