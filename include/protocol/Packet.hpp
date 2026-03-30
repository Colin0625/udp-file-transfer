#pragma once

#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"
#include <vector>
#include <cstddef>
#include <span>

class Packet {
private:
    PacketHeader header_;
    std::vector<std::byte> payload_;
    static uint32_t checksum(std::span<const std::byte> bytes);

public:
    Packet();
    Packet(MessageType type, uint32_t sequence_number, std::span<const std::byte> payload);
    static Packet parse(std::span<const std::byte> bytes);
    std::vector<std::byte> serialize() const;
};