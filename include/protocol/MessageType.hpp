#pragma once

#include <cstdint>

enum class MessageType : uint8_t {
    SYN         = 0,
    ACK         = 1,
    SYNACK      = 2,
    GET         = 3,
    METADATA    = 4,
    DATA        = 5,
    COMPLETE    = 6,
    RETRANSMIT  = 7,
    ERROR       = 8,
    CLOSE       = 9,
    CLOSEACK    = 10,

};