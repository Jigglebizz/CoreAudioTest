#pragma once

#define SAFE_RELEASE(punk) \
                if ((punk) != NULL) \
                  { (punk)->Release(); (punk) = NULL; }
