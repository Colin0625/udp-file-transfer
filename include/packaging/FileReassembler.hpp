#include <string>
#include <fstream>

#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"
#include "protocol/MessageType.hpp"


class FileReassembler {
private:
    std::string current_filename_;
    std::ofstream current_file_;



public:
    FileReassembler();
    void open(const std::string& filename, uint64_t file_size);
    int write(Packet packet);


};