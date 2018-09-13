#include <stdafx.h>
#include "WinDrivers.h"

const CLSID WinDrivers::CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID WinDrivers::IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID WinDrivers::IID_IAudioClient = __uuidof(IAudioClient);
const IID WinDrivers::IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

const unsigned WinDrivers::REFTIMES_PER_SEC = 10'000'000;
const unsigned WinDrivers::REFTIMES_PER_MS = 10'000;