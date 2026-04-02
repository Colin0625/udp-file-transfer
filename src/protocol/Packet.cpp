#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"
#include <stdexcept>
#include <arpa/inet.h>


Packet::Packet(MessageType type, uint32_t sequence_number, std::span<const std::byte> payload)
 : header_(type, payload.size(), sequence_number), payload_(payload.begin(), payload.end()) 
{
    header_.checksum_ = checksum(header_, payload_);
}

Packet::Packet(const PacketHeader& header, std::span<const std::byte> payload)
 : header_(header), payload_(payload.begin(), payload.end()) 
{}

Packet Packet::parse(std::span<const std::byte> bytes) {
    MessageType type = static_cast<MessageType>(bytes[0]);

    uint16_t payload_size =
        (static_cast<uint16_t>(std::to_integer<uint8_t>(bytes[1])) << 8) |
         static_cast<uint16_t>(std::to_integer<uint8_t>(bytes[2]));

    uint32_t sequence_number =
        (static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[3])) << 24) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[4])) << 16) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[5])) << 8)  |
         static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[6]));

    uint32_t checksum =
        (static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[7])) << 24) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[8])) << 16) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[9])) << 8)  |
         static_cast<uint32_t>(std::to_integer<uint8_t>(bytes[10]));

    return Packet(
        PacketHeader(type, payload_size, sequence_number, checksum),
        bytes.subspan(PacketHeader::header_size, payload_size)
    );
}

std::vector<std::byte> Packet::serialize() const {
    std::vector<std::byte> buffer(PacketHeader::header_size + payload_.size());

    buffer[0] = static_cast<std::byte>(header_.type_);

    buffer[1] = static_cast<std::byte>((header_.payload_size_ >> 8) & 0xFF);
    buffer[2] = static_cast<std::byte>(header_.payload_size_ & 0xFF);

    buffer[3] = static_cast<std::byte>((header_.sequence_number_ >> 24) & 0xFF);
    buffer[4] = static_cast<std::byte>((header_.sequence_number_ >> 16) & 0xFF);
    buffer[5] = static_cast<std::byte>((header_.sequence_number_ >> 8) & 0xFF);
    buffer[6] = static_cast<std::byte>(header_.sequence_number_ & 0xFF);

    buffer[7] = static_cast<std::byte>((header_.checksum_ >> 24) & 0xFF);
    buffer[8] = static_cast<std::byte>((header_.checksum_ >> 16) & 0xFF);
    buffer[9] = static_cast<std::byte>((header_.checksum_ >> 8) & 0xFF);
    buffer[10] = static_cast<std::byte>(header_.checksum_ & 0xFF);

    std::copy(payload_.begin(), payload_.end(), buffer.begin() + PacketHeader::header_size);

    return buffer;
}

uint32_t Packet::checksum(PacketHeader header, std::span<const std::byte> bytes) {
    uint32_t res{};
    res += static_cast<uint32_t>(header.type_);
    res += static_cast<uint32_t>(header.payload_size_);
    res += static_cast<uint32_t>(header.sequence_number_);
    for (std::byte b : bytes) {
        res = res + static_cast<uint8_t>(b);
    }
    return res;
}

bool Packet::compare_checksum(PacketHeader header, std::span<const std::byte> bytes) {
    uint32_t csum = header.checksum_;
    if (Packet::checksum(header, bytes) != csum) {
        return false;
    }
    return true;
}