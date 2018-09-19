#include <stdafx.h>
#include "Device.h"

#include "objectModel/RenderingChain.h"

using namespace speakerService;
using namespace std::chrono_literals;

uint64_t Device::idCounter = 0;
const std::chrono::milliseconds Device::cDefaultLatency = 3ms;

Device::Device() noexcept
    : mName(L"Unknown"),
    mUid(L""),
    mWinDevice(NULL),
    mBufferSize(cDefaultBufferSize),
    mSampleRate(cDefaultSampleRate),
    mBitsPerSample(cDefaultBitsPerSample),
    mBytesPerFrame(cDefaultBytesPerFrame),
    mNumChannels(1),
    mLatency(cDefaultLatency),
    mRenderClient(NULL),
    mAudioClient(NULL),
    mOpened(false),
    mRenderingChain(std::make_unique<objectModel::RenderingChain>())
{}


Device::Device(Device& Other) noexcept :
    Device()
{
    swap(*this, Other);
}

Device&
Device::operator=(Device Other) noexcept {
    swap(*this, Other);
    return *this;
}

Device::Device( Device&& Other) noexcept :
    Device()
{
    swap(*this, Other);
}

void speakerService::swap(Device & A, Device & B) noexcept
{
    using std::swap;
    swap(A.mName, B.mName);
    swap(A.mUid, B.mUid);
    swap(A.mAppId, B.mAppId);
    swap(A.mBufferSize, B.mBufferSize);
    swap(A.mSampleRate, B.mSampleRate);
    swap(A.mBitsPerSample, B.mBitsPerSample);
    swap(A.mBytesPerFrame, B.mBytesPerFrame);
    swap(A.mNumChannels, B.mNumChannels);
    swap(A.mWinDevice, B.mWinDevice);
    swap(A.mRenderClient, B.mRenderClient);
    swap(A.mAudioClient, B.mAudioClient);
    swap(A.mLatency, B.mLatency);
    swap(A.mOpened, B.mOpened);
    swap(A.mRenderingChain, B.mRenderingChain);
    swap(A.mRenderingThread, B.mRenderingThread);
    B.mCloseRequested 
        = A.mCloseRequested.exchange(B.mCloseRequested);
    B.mRenderThreadClosed 
        = A.mRenderThreadClosed.exchange(B.mRenderThreadClosed);
}


objectModel::RenderingChain& 
Device::getChain() noexcept {
    return *mRenderingChain;
}

Device::Device(IMMDevice *pDevice) noexcept
  : Device()
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
        mBytesPerFrame = pwfx->nBlockAlign;

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
    float** fBuf = new float*[mNumChannels]();
    for (unsigned i = 0; i < mNumChannels; ++i) {
        fBuf[i] = new float[mBufferSize]();
    }

    mRenderThreadClosed = false;

    HRESULT hr;
    REFERENCE_TIME hnsActualDuration = 
        static_cast<REFERENCE_TIME>(
            static_cast<double>(WinDrivers::REFTIMES_PER_SEC) *
            static_cast<double>(mBufferSize) / 
            static_cast<double>(mSampleRate)
        );
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
                throw std::runtime_error("Error getting padding. Error#" + std::to_string(hr));

            numFramesAvailable = mBufferSize - numFramesPadding;
            if (numFramesAvailable < 1) {
                continue;
            }

            hr = mRenderClient->GetBuffer(numFramesAvailable, &pBuf);
            if FAILED(hr)
                throw std::runtime_error(
                    "Error getting render buffer: " + 
                    WinDrivers::audioClientErrorString(hr)
                );

            mRenderingChain->render(fBuf, mNumChannels, numFramesAvailable, *this);
            floatToWinBuf(fBuf, pBuf, numFramesAvailable);

            hr = mRenderClient->ReleaseBuffer(numFramesAvailable, flags);
            if (FAILED(hr))
                throw std::runtime_error("Error releasing render buffer: " + 
                    WinDrivers::audioClientErrorString( hr));
        }
        catch (const std::runtime_error& e) {
            std::wcout << e.what() << std::endl;
            close();
        }
    }

    mRenderThreadClosed = true;
}

#pragma endregion

void
Device::floatToWinBuf( float** FloatBuf, BYTE* WinBuf, size_t NumFrames) const noexcept {
    // From https://docs.microsoft.com/en-us/windows/desktop/api/Audioclient/nn-audioclient-iaudiorenderclient
    // The size in bytes of an audio frame equals the number of channels in 
    // the stream multiplied by the sample size per channel. For example, 
    // the frame size is four bytes for a stereo (2-channel) stream with 
    // 16-bit samples. A packet always contains an integral number of audio 
    // frames.
    double halfMax = static_cast<double>(std::numeric_limits<uint32_t>::max() / 2);

    for (unsigned f = 0; f < NumFrames; ++f) {
        for (unsigned n = 0; n < mNumChannels; ++n) {
            (uint32_t&)WinBuf[f * mBytesPerFrame] = static_cast<uint32_t>(FloatBuf[0][f] * halfMax + halfMax);
        }
    }
}
