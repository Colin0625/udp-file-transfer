#include <thread>

#include "transfer/ServerSession.hpp"

ServerSession::ServerSession() : endpoint_{}, server_address_{} {}

void ServerSession::bind_server(int port) {
    SocketAddress addr = SocketAddress::any(port);
    endpoint_.bind_endpoint(addr);
}



