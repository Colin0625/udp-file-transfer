#include <string>
#include <fstream>
#include <vector>

#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"
#include "protocol/Packet.hpp"

class FilePackager {
private:
    std::ifstream file_;
    uint64_t file_size_bytes_;
    uint64_t number_of_packets_;
    std::string filename_;

public:
    FilePackager();
    FilePackager(const std::string& filename);
    Packet get_packet(uint64_t packet_number);


    static const uint16_t max_payload_size_ = 1024 - PacketHeader::header_size_;
    const std::string& get_filename() const;
    const uint64_t& get_file_size() const;
    const uint64_t& get_num_of_packets() const;
};