#include "net/UdpSocket.hpp"
#include "packaging/FileReassembler.hpp"
#include <string>

class ClientSession {
private:
    UdpSocket socket_;
    SocketAddress server_address_;
    FileReassembler file_reassembler_;

public:
    ClientSession();
    void connect_to_server(uint16_t port, const std::string& ip_addr);




};