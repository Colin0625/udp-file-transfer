#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"

// Must figure out the constructors
// One constructor for when I want to receive
// Another for when I want to send
// A default constructor because why? Because I want to use it in parse I think?

Packet::Packet() : header_{}, payload_{} {}

Packet::Packet(MessageType type, uint32_t sequence_number, std::span<const std::byte> payload)
 : header_(type, payload.size(), sequence_number), payload_(payload.begin(), payload.end()) 
{}

Packet Packet::parse(std::span<const std::byte> bytes) {

}