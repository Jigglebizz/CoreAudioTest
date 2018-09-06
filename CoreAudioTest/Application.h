#pragma once

#include <thread>

#include "Context.h"

class Application {
private:
    bool mExitRequested;
    Context mContext;

    void Initialize();
    void Loop();
public:
    explicit Application() noexcept;

    void Run();
};
