#include "include/transfer/FilePackager.hpp"
#include <vector>
#include <iostream>

int main() {
    FilePackager p("testfile.txt");
    std::vector<char> vec = p.get_packet(0);
    for (char c : vec) {
        std::cout << c;
    }
    std::cout << std::endl;



    return 0;
}
