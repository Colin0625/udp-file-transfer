#pragma once

#include "Endpoint.hpp"

class Server : public Endpoint {
private:
    int port;
    sockaddr_in client_addr{};
public:
    Server(int _port);
    void start();
    void receive_message();
};