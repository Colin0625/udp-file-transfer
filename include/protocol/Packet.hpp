#pragma once

#include <vector>
#include <cstddef>
#include <span>
#include <sys/types.h>

#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"
#include "net/SocketAddress.hpp"

class Packet {
private:
    PacketHeader header_;
    SocketAddress sender_address_;
    std::vector<std::byte> payload_;
    static uint32_t checksum(PacketHeader header, std::span<const std::byte> bytes);
    static bool compare_checksum(PacketHeader header, std::span<const std::byte> bytes);
public:
    Packet(MessageType type, uint32_t sequence_number, std::span<const std::byte> payload);
    Packet(const PacketHeader& header, const SocketAddress& addr, std::span<const std::byte> payload);
    Packet(MessageType type);
    static Packet parse(std::span<const std::byte> bytes, ssize_t len, SocketAddress addr);
    std::vector<std::byte> serialize() const;


    const std::vector<std::byte>& get_payload() const;
    const PacketHeader& get_header() const;
    void print() const;
    static const uint16_t max_packet_size = 1024;
};