#include <stdafx.h>
#include "Device.h"

using namespace speakerService;

uint64_t Device::idCounter = 0;

Device::Device() noexcept
    : mName(L"Unknown"),
      mUid(L""),
      mWinDevice(NULL),
      mBufferSize( cDefaultBufferSize), 
      mSampleRate( cDefaultSampleRate)
{}


Device::Device( Device&& Other) noexcept :
    mName( Other.mName), 
    mUid( Other.mUid),
    mAppId( Other.mAppId),
    mWinDevice( Other.mWinDevice),
    mBufferSize( Other.mBufferSize),
    mSampleRate( Other.mSampleRate)
{
    Other.mWinDevice = NULL;
    Other.mName = L"Unknown";
    Other.mAppId = std::numeric_limits<uint64_t>::max();
    Other.mUid = L"";
}

Device::Device(IMMDevice *pDevice) noexcept
  : mName(L"Unknown"), mWinDevice(NULL), mUid( L"")
{
    HRESULT hr = S_OK;
    LPWSTR pwszID = NULL;
    IPropertyStore *pProps = NULL;

    try {
        hr = pDevice->GetId(&pwszID);
        if (FAILED(hr))
            throw std::runtime_error("Could not get device ID of " +
            (mAppId == -1) ? "default device" : "device #" + std::to_string(mAppId));

        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (FAILED(hr))
            throw std::runtime_error("Could not open property store for " +
            (idCounter == -1) ? "default device" : "device #" + std::to_string(mAppId));

        PROPVARIANT varName;
        PropVariantInit(&varName);
        hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
        if (FAILED(hr))
            throw std::runtime_error("Failed to get property 'Friendly Name' for " +
            (mAppId == -1) ? "default device" : "device #" + std::to_string(mAppId));

        mName = varName.pwszVal;
        mUid = pwszID;
        mWinDevice = pDevice;

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

    mAppId = idCounter++;
}

Device::~Device() {
    if (mWinDevice != NULL) {
        mWinDevice->Release();
    }
}

void
Device::Render( const AudioBuffer<uint32_t>& Buf) {
    HRESULT hr;
    //REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME hnsActualDuration;
}