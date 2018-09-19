#pragma once

#include <vector>

#include "AbstractService.h"
#include "speakerService/Device.h"

/**
 * This service is responsible for connecting to devices, and
 * retrieving speakerService::Devices to represent them.
 *
 * A speakerService::Device may be used to stream audio out.
 */
class SpeakerService : public AbstractService {
private:
    IMMDeviceEnumerator *mEnumerator;
public:
    SpeakerService( Context& C);
    ~SpeakerService();

    speakerService::Device GetDefaultDevice() const;
    std::vector<speakerService::Device > GetDevices() const;
};
