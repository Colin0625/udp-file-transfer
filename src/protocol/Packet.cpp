#include <stdexcept>
#include <arpa/inet.h>
#include <iostream>

#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"


Packet::Packet(MessageType type, uint32_t sequence_number, std::span<const std::byte> payload)
 : header_(type, payload.size(), sequence_number), payload_(payload.begin(), payload.end()) 
{
    header_.checksum_ = checksum(header_, payload_);
}

Packet::Packet(const PacketHeader& header, std::span<const std::byte> payload)
 : header_(header), payload_(payload.begin(), payload.end()) 
{}

Packet::Packet(MessageType type)
 : header_(type), payload_{}
{}

Packet Packet::parse(std::span<const std::byte> bytes, ssize_t len) {
    if (len < 0) {
        throw std::runtime_error("Failed to receive bytes");
    }

    std::span<const std::byte> msg = bytes.subspan(0, len);

    MessageType type = static_cast<MessageType>(msg[0]);

    uint16_t payload_size =
        (static_cast<uint16_t>(std::to_integer<uint8_t>(msg[1])) << 8) |
         static_cast<uint16_t>(std::to_integer<uint8_t>(msg[2]));

    uint32_t sequence_number =
        (static_cast<uint32_t>(std::to_integer<uint8_t>(msg[3])) << 24) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(msg[4])) << 16) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(msg[5])) << 8)  |
         static_cast<uint32_t>(std::to_integer<uint8_t>(msg[6]));

    uint32_t checksum =
        (static_cast<uint32_t>(std::to_integer<uint8_t>(msg[7])) << 24) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(msg[8])) << 16) |
        (static_cast<uint32_t>(std::to_integer<uint8_t>(msg[9])) << 8)  |
         static_cast<uint32_t>(std::to_integer<uint8_t>(msg[10]));

    return Packet(
        PacketHeader(type, payload_size, sequence_number, checksum),
        msg.subspan(PacketHeader::header_size_, payload_size)
    );
}

std::vector<std::byte> Packet::serialize() const {
    std::vector<std::byte> buffer(PacketHeader::header_size_ + payload_.size());

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

    std::copy(payload_.begin(), payload_.end(), buffer.begin() + PacketHeader::header_size_);

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

const std::vector<std::byte>& Packet::get_payload() const {
    return payload_;
}

const PacketHeader& Packet::get_header() const {
    return header_;
}

void Packet::print() const {
    std::cout << "Packet contents" << std::endl;
    std::cout << "Packet type: " << static_cast<int>(header_.type_) << std::endl;
    std::cout << "Packet payload size: " << header_.payload_size_ << std::endl;
    std::cout << "Packet sequence number: " << header_.sequence_number_ << std::endl;
    std::cout << "Packet checksum: " << header_.checksum_ << std::endl;
    std::cout << std::endl;
}