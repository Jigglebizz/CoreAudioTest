#include <stdafx.h>
#include "Device.h"

using namespace speakerService;
using namespace std::chrono_literals;

uint64_t Device::idCounter = 0;
const std::chrono::milliseconds Device::cDefaultLatency = 3ms;

Device::Device() noexcept
    : mName(L"Unknown"),
      mUid(L""),
      mWinDevice(NULL),
      mBufferSize( cDefaultBufferSize), 
      mSampleRate( cDefaultSampleRate),
      mBitsPerSample( cDefaultBitsPerSample),
      mNumChannels( 1),
      mLatency( cDefaultLatency),
      mRenderClient( NULL),
      mAudioClient( NULL),
      mOpened(false)
{}


Device::Device( Device&& Other) noexcept :
    mName( Other.mName), 
    mUid( Other.mUid),
    mAppId( Other.mAppId),
    mWinDevice( Other.mWinDevice),
    mBufferSize( Other.mBufferSize),
    mSampleRate( Other.mSampleRate),
    mBitsPerSample( Other.mBitsPerSample),
    mNumChannels( Other.mNumChannels),
    mLatency( Other.mLatency),
    mOpened( Other.mOpened),
    mRenderClient( Other.mRenderClient),
    mAudioClient( Other.mAudioClient)
{
    Other.mWinDevice = NULL;
    Other.mRenderClient = NULL;
    Other.mAudioClient = NULL;
    Other.mName = L"Unknown";
    Other.mAppId = std::numeric_limits<uint64_t>::max();
    Other.mUid = L"";
    Other.mOpened = false;
}

Device::Device(IMMDevice *pDevice) noexcept
  : mName(L"Unknown"), 
    mWinDevice(NULL),
    mRenderClient(NULL),
    mAudioClient(NULL),
    mUid( L""),
    mBufferSize(cDefaultBufferSize),
    mSampleRate(cDefaultSampleRate),
    mBitsPerSample(cDefaultBitsPerSample),
    mLatency(cDefaultLatency),
    mOpened( false)
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
    if (mOpened)
        close();

    if (mRenderClient != NULL) {
        mRenderClient->Release();
    }
    if (mAudioClient != NULL) {
        mAudioClient->Release();
    }
    if (mWinDevice != NULL) {
        mWinDevice->Release();
    }
}

#pragma region
// This region is mostly boilerplate from
// https://docs.microsoft.com/en-us/windows/desktop/CoreAudio/rendering-a-stream

void Device::open() {
    assert( !mOpened && "Device already opened");

    HRESULT hr;
    REFERENCE_TIME hnsRequestedDuration = static_cast<REFERENCE_TIME>(
        static_cast<double>(WinDrivers::REFTIMES_PER_MS) * 
        static_cast<double>(mLatency.count()));
    WAVEFORMATEX *pwfx = NULL;

    try {
        hr = mWinDevice->Activate( 
                            WinDrivers::IID_IAudioClient, CLSCTX_ALL,
                            NULL, (void**)&mAudioClient);
        if (FAILED(hr))
            throw std::runtime_error("Could not activate device");

        hr = mAudioClient->GetMixFormat(&pwfx);
        if (FAILED(hr))
            throw std::runtime_error("Could not get mix format");

        mSampleRate = pwfx->nSamplesPerSec;
        mBitsPerSample = pwfx->wBitsPerSample;
        mNumChannels = pwfx->nChannels;

        hr = mAudioClient->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            NULL, hnsRequestedDuration, 0, pwfx, NULL);

        if (FAILED(hr))
            throw std::runtime_error("Could not initialize device");

        hr = mAudioClient->GetBufferSize(&mBufferSize);
        if (FAILED(hr))
            throw std::runtime_error("Could not get buffer size");

        hr = mAudioClient->GetService(
            WinDrivers::IID_IAudioRenderClient,
            (void**)&mRenderClient);
        if (FAILED(hr))
            throw std::runtime_error("Could not get render client");

        hr = mAudioClient->Start();
        if (FAILED(hr))
            throw std::runtime_error("Could not start audio client");

        mOpened = true;
        mCloseRequested = false;
        mRenderingThread = std::thread( &Device::render, this);
    }
    catch ( const std::runtime_error&) {
        CoTaskMemFree(pwfx);
        SAFE_RELEASE(mAudioClient);
        SAFE_RELEASE(mRenderClient);
        throw;
    }
    CoTaskMemFree(pwfx);
}

void
Device::close() {
    assert(mOpened && "Can not close unopened device");

    mCloseRequested = true;
    if (mRenderingThread.joinable())
        mRenderingThread.join();

    while (!mRenderThreadClosed) {}

    mAudioClient->Stop();
    SAFE_RELEASE(mAudioClient);
    mAudioClient = NULL;
    SAFE_RELEASE(mRenderClient);
    mRenderClient = NULL;

    mOpened = false;
}

void
Device::render() noexcept {
    assert(mOpened && "Device must be opened first");
    BYTE* pBuf;
    float** fBuf = (float**)malloc( mNumChannels * sizeof(float*));
    for (unsigned i = 0; i < mNumChannels; ++i) {
        fBuf[i] = (float*)malloc(mBufferSize * sizeof(float));
    }

    mRenderThreadClosed = false;

    HRESULT hr;
    REFERENCE_TIME hnsActualDuration = 
        (double)WinDrivers::REFTIMES_PER_SEC *
        mBufferSize / mSampleRate;
    DWORD flags = 0;

    UINT32 numFramesAvailable;
    UINT32 numFramesPadding;

    LPVOID pvReserved = NULL;
    hr = CoInitializeEx(pvReserved, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);

    while (!mCloseRequested) {
        try {
            std::this_thread::sleep_for( 
                std::chrono::milliseconds(
                    hnsActualDuration / WinDrivers::REFTIMES_PER_MS / 2
                )
            );

            hr = mAudioClient->GetCurrentPadding(&numFramesPadding);
            if FAILED(hr)
                throw std::runtime_error("Error getting padding");

            numFramesAvailable = mBufferSize - numFramesPadding;

            hr = mRenderClient->GetBuffer(mBufferSize, &pBuf);
            if FAILED(hr)
                throw std::runtime_error("Error getting render buffer");

            mRenderingChain.render(fBuf, mNumChannels, numFramesAvailable);
            floatToWinBuf(fBuf, pBuf);

            hr = mRenderClient->ReleaseBuffer(mBufferSize, flags);
            if (FAILED(hr))
                throw std::runtime_error("Error releasing render buffer");
        }
        catch (const std::runtime_error&) {
            close();
        }
    }

    mRenderThreadClosed = true;
}

#pragma endregion

void
Device::floatToWinBuf( float** FloatBuf, BYTE* WinBuf) const noexcept {

}