#pragma once

#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"
#include <vector>
#include <cstddef>
#include <span>
#include <sys/types.h>

class Packet {
private:
    PacketHeader header_;
    std::vector<std::byte> payload_;
    static uint32_t checksum(PacketHeader header, std::span<const std::byte> bytes);
    static bool compare_checksum(PacketHeader header, std::span<const std::byte> bytes);
public:
    Packet(MessageType type, uint32_t sequence_number, std::span<const std::byte> payload);
    Packet(const PacketHeader& header, std::span<const std::byte> payload);
    static Packet parse(std::span<const std::byte> bytes, ssize_t len);
    std::vector<std::byte> serialize() const;


    const std::vector<std::byte>& get_payload() const;
    const PacketHeader& get_header() const;
    void print() const;
};