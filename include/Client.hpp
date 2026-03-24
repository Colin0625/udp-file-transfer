#pragma once

#include "Endpoint.hpp"


class Client : public Endpoint {
private:
    int port;
    std::string ip;
public:
    Client(int _port, std::string _ip);
    void send_message(MessageType _t);
};