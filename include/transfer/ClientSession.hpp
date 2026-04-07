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
    int connect_to_server(uint16_t port, const std::string& ip_addr);
    int request_file(const std::string& filename);
    int receive_file();
    int receive_packets(std::vector<std::byte>& buffer);
    int save_packets();
    static uint64_t metadata_to_uint64(std::span<std::byte> metadata);
};