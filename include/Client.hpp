#pragma once

#include "Endpoint.hpp"


class Client : public Endpoint {
private:
    int port;
    std::string ip;
public:
    Client();
    int connect_to_server(int _port, std::string _ip);
    int request_file(std::string& _filename);
    int receive_file_size(sockaddr_in& _addr);
    int receive_file_data(sockaddr_in& _addr, int _numpackets, int _filesize, std::string& _filename);
    
};