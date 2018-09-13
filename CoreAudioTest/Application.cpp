#include <stdafx.h>
#include "Application.h"

#include <chrono>
#include <iostream>

#include "services/SpeakerService.h"

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
}

void
Application::Initialize() {
    mContext.InitializeService<SpeakerService>();
    auto devices = mContext.GetService<SpeakerService>()->GetDevices();

    for (auto& dev : devices) {
        std::wcout << dev.getUid() << " ... " << dev.getName() << std::endl;
    }

    auto defaultDevice = mContext.GetService<SpeakerService>()->GetDefaultDevice();
    std::wcout << std::endl << "Default device is: " 
               << defaultDevice.getUid() << " ... " 
               << defaultDevice.getName() << std::endl;
}

void
Application::Loop() {

}