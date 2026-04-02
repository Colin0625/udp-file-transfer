#include "include/transfer/FilePackager.hpp"
#include <string>
#include <fstream>
#include <cstddef>

FilePackager::FilePackager(const std::string& filename) : file_(filename, std::ios::binary) {
    file_.seekg(0, std::ios::end);
    file_size_bytes_ = file_.tellg();
    file_.seekg(0, std::ios::beg);

    number_of_packets_ = file_size_bytes_ / PacketHeader::header_size_;
    if (file_size_bytes_ % PacketHeader::header_size_) {
        number_of_packets_++;
    }
}

std::vector<char> FilePackager::get_packet(uint64_t packet_number) {
    uint64_t start = packet_number * FilePackager::max_payload_size_;
    uint64_t end = (packet_number + 1) * FilePackager::max_payload_size_;
    if (end > file_size_bytes_) {
        end = file_size_bytes_ - 1;
    }

    file_.seekg(start);
    std::vector<char> s(FilePackager::max_payload_size_);
    file_.get(s.data(), end-start);
    return s;
}