#include <thread>

#include "transfer/ClientSession.hpp"
#include "utility/Queue.hpp"
#include "protocol/Packet.hpp"

ClientSession::ClientSession() : endpoint_{}, server_address_{} {}


ssize_t ClientSession::send(Packet p, SocketAddress addr) {
    return endpoint_.send_packet(p, addr);
}