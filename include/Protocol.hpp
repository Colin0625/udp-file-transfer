#pragma once

#include <cstdint>

enum class MessageType : std::uint8_t {
    CONNECT     = 1,
    OK          = 2,
    GET         = 3,
    SIZE        = 4,
    DATA        = 5,
    CLOSE       = 6,
    ERROR       = 7,
};