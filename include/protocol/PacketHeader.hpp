#pragma once

#include "protocol/MessageType.hpp"

struct PacketHeader {
    MessageType type;
    uint32_t sequence_number;
    uint32_t checksum;
    uint16_t payload_size;
};