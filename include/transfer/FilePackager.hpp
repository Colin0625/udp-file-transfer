#include "include/protocol/PacketHeader.hpp"
#include "include/protocol/MessageType.hpp"
#include <string>
#include <fstream>

class FilePackager {
private:
    std::ifstream file_;
    uint64_t file_size_bytes_;
    uint64_t number_of_packets_;

public:
    FilePackager(std::string filename);








    static const uint16_t max_payload_size_ = 1024 - PacketHeader::header_size_;

};