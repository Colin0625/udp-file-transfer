#include <vector>
#include <iostream>
#include <span>
#include <cstddef>

#include "packaging/FilePackager.hpp"
#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"
#include "packaging/FileReassembler.hpp"


int main() {
    FilePackager p("test-files/cpp.png");
    std::cout << "Opened " << p.get_filename() << std::endl;
    std::cout << "File size: " << p.get_file_size() << std::endl;
    std::cout << "Number of packets: " << p.get_num_of_packets() << std::endl;
    std::cout << std::endl;

    
    FileReassembler reass{};
    reass.open("test-output/cpp.png", p.get_file_size());

    for (int i = 0; i < p.get_num_of_packets(); i++) {
        Packet pack = p.get_packet(i);
        reass.write(pack);
    }

    return 0;
}
