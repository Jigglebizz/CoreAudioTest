#pragma once

#include <vector>

#include "AbstractService.h"
#include "speakerService/Device.h"

class SpeakerService : public AbstractService {
private:
    IMMDeviceEnumerator *mEnumerator;
public:
    SpeakerService( Context& C);
    ~SpeakerService();

    speakerService::Device GetDefaultDevice() const;
    std::vector<speakerService::Device > GetDevices() const;
};
