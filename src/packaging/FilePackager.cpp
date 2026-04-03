#include "packaging/FilePackager.hpp"
#include <string>
#include <fstream>
#include <cstddef>


FilePackager::FilePackager() : file_{}, file_size_bytes_{}, number_of_packets_{}, filename_{} {}

FilePackager::FilePackager(const std::string& filename) : file_(filename, std::ios::binary), filename_(filename) {
    file_.seekg(0, std::ios::end);
    file_size_bytes_ = file_.tellg();
    file_.seekg(0, std::ios::beg);

    number_of_packets_ = file_size_bytes_ / FilePackager::max_payload_size_;
    if (file_size_bytes_ % FilePackager::max_payload_size_) {
        number_of_packets_++;
    }
}

Packet FilePackager::get_packet(uint64_t packet_number) {
    uint64_t start = packet_number * FilePackager::max_payload_size_;
    uint64_t end = (packet_number + 1) * FilePackager::max_payload_size_;
    if (end > file_size_bytes_) {
        end = file_size_bytes_;
    }

    file_.seekg(start);
    std::vector<char> vec(end-start);
    file_.read(vec.data(), end-start);
    std::span<std::byte> buf(reinterpret_cast<std::byte*>(vec.data()), vec.size());
    return Packet(MessageType::DATA, packet_number, buf);
}

const std::string& FilePackager::get_filename() const {
    return filename_;
}

const uint64_t& FilePackager::get_file_size() const {
    return file_size_bytes_;
}

const uint64_t& FilePackager::get_num_of_packets() const {
    return number_of_packets_;
}