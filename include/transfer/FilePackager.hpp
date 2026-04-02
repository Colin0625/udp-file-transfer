#include "include/protocol/PacketHeader.hpp"
#include "include/protocol/MessageType.hpp"
#include <string>
#include <fstream>
#include <vector>

class FilePackager {
private:
    std::ifstream file_;
    uint64_t file_size_bytes_;
    uint64_t number_of_packets_;

public:
    FilePackager(const std::string& filename);
    std::vector<char> get_packet(uint64_t packet_number);







    static const uint16_t max_payload_size_ = 1024 - PacketHeader::header_size_;

};