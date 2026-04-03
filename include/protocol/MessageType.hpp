#pragma once

#include <cstdint>

enum class MessageType : uint8_t {
    SYN         = 1,
    ACK         = 2,
    SYNACK      = 3,
    GET         = 4,
    METADATA    = 5,
    DATA        = 6,
    COMPLETE    = 7,
    RETRANSMIT  = 8,
    ERROR       = 9,

};