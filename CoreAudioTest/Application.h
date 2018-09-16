#pragma once

#include "Context.h"
#include "services/speakerService/Device.h"

class Application {
private:
    bool mExitRequested;
    Context mContext;

    speakerService::Device mDevice;

    void Initialize();
    void Loop();
public:
    explicit Application() noexcept;

    void Run();
};
