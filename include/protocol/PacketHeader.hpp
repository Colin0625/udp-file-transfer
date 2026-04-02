#pragma once

#include "protocol/MessageType.hpp"
#include <cstdint>

struct PacketHeader {
    MessageType type_;
    uint16_t payload_size_;
    uint32_t sequence_number_;
    uint32_t checksum_;

    static const uint8_t header_size = 11;
    PacketHeader() : type_{}, payload_size_{}, sequence_number_{}, checksum_{} {}

    PacketHeader(MessageType type, uint16_t payload_size, uint32_t sequence_number, uint32_t checksum) 
    : type_(type), payload_size_(payload_size), sequence_number_(sequence_number), checksum_(checksum) {}
    
    PacketHeader(MessageType type, uint16_t payload_size, uint32_t sequence_number) 
    : type_(type), payload_size_(payload_size), sequence_number_(sequence_number), checksum_{} {}
};