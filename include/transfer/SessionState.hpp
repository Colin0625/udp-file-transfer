#pragma once

#include <cstdint>

enum class SessionState : uint8_t {
    OFF             = 0,
    IDLE            = 1,
    CONNECTING      = 2,
    CONNECTED       = 3,
    METATRANSFER    = 4,
    TRANSFERRING    = 5,
    CLOSING         = 6
};