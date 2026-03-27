#pragma once

#include "Endpoint.hpp"

class Server : public Endpoint {
private:
    int port;
    sockaddr_in client_addr{};
public:
    Server();
    void start(int _port);
    void receive_message();
    ssize_t handle_conn_message(sockaddr_in& _addr);
    ssize_t handle_get_message(sockaddr_in& _addr, std::span<std::byte> _buffer);
    ssize_t handle_close_message(sockaddr_in& _addr);
};