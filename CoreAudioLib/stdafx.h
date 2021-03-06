#pragma once

#define NOMINMAX

// Windows drivers includes
#include <winerror.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <Audioclient.h>
#include <cassert>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <thread>
#include <atomic>
#include <list>
#include <iostream>

#ifdef _UNIT_TEST
#define testable_public public:
#else
#define testable_public
#endif