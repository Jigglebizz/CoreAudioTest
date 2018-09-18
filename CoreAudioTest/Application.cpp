#include <stdafx.h>
#include "Application.h"

#include "services/SpeakerService.h"
#include "objectModel/RenderingChain.h"
#include "plugins/DigitalNoise.h"

using namespace std::chrono_literals;

Application::Application() noexcept
    : mExitRequested( false)
{}

void
Application::Run() {
    Initialize();

    while ( !mExitRequested) {
        Loop();
        std::this_thread::sleep_for(1ms);
    }

     mDevice.close();
}

void
Application::Initialize() {
    mContext.InitializeService<SpeakerService>();
    auto devices = mContext.GetService<SpeakerService>()->GetDevices();

    for (auto& dev : devices) {
        std::wcout << dev.getUid() << " ... " << dev.getName() << std::endl;
    }

    mDevice = mContext.GetService<SpeakerService>()->GetDefaultDevice();
    std::wcout << std::endl << "Default device is: " 
               << mDevice.getUid() << " ... " 
               << mDevice.getName() << std::endl;

    mDevice.setLatency(1ms);

    plugins::DigitalNoise* noiseGen = new plugins::DigitalNoise();

    auto& chain = mDevice.getChain();
    chain.insert(0, noiseGen);

    mDevice.open();

    std::wcout << "Sample rate: " << mDevice.getSampleRate() << std::endl;
    std::wcout << "Bit depth: " << mDevice.getBitDepth() << std::endl;
    std::wcout << "Buffer size: " << mDevice.getBufferSize() << std::endl;
    if (mDevice.getNumChannels() == 1)
        std::wcout << "Mono" << std::endl;
    else if (mDevice.getNumChannels() == 2)
        std::wcout << "Stereo" << std::endl;

    std::wcout << "Buffer latency: " << 
                  1000.0 * (float)mDevice.getBufferSize() / 
                  (float)mDevice.getSampleRate() << "ms" << std::endl;
}

void
Application::Loop() {

}