#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#include "packaging/FileReassembler.hpp"
#include "packaging/FilePackager.hpp"
#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"


FileReassembler::FileReassembler() {

}

void FileReassembler::open(const std::string& filename, uint64_t file_size) {
    current_filename_ = filename;
    current_file_ = std::ofstream(filename);
    std::filesystem::resize_file(filename, file_size);
}

int FileReassembler::write(Packet packet) {
    if (packet.get_header().type_ == MessageType::DATA) {
        current_file_.seekp(packet.get_header().sequence_number_ * FilePackager::max_payload_size_);
        current_file_.write(reinterpret_cast<const char*>(packet.get_payload().data()), packet.get_payload().size());
        return 0;
    }
    return 1;
}