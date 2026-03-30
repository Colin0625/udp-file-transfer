#pragma once

#include <cstdint>

enum class MessageType : uint8_t {
    SYN         = 1,
    ACK         = 2,
    GET         = 3,
    METADATA    = 4,
    DATA        = 5,
    COMPLETE    = 6,
    RETRANSMIT  = 7,
    ERROR       = 8
};