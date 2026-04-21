#pragma once

#include <cstdint>

enum class SessionState : uint8_t {
    OFF             = 0,
    IDLE            = 1,
    CONNECTING      = 2,
    CONNECTED       = 3,
    TRANSFERRING    = 4,
    CLOSING         = 5
};