#include "Server.hpp"

int main() {
    Server server;
    server.start(5000);
    server.receive_message();
    return 0;
}