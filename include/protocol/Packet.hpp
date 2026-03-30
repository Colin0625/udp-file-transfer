#pragma once

#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"
#include <vector>
#include <cstddef>

class Packet {
private:
    PacketHeader header_;
    std::vector<std::byte> payload_;

public:
    



};