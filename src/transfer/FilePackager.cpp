#include "include/transfer/FilePackager.hpp"
#include <string>
#include <fstream>

FilePackager::FilePackager(std::string filename) : file_(filename, std::ios::binary) {
    file_.seekg(0, std::ios::end);
    file_size_bytes_ = file_.tellg();
    file_.seekg(0, std::ios::beg);

    number_of_packets_ = file_size_bytes_ / PacketHeader::header_size_;
    if (file_size_bytes_ % PacketHeader::header_size_) {
        number_of_packets_++;
    }

    
}