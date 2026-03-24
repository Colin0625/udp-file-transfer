#include "Server.hpp"

int main() {
    Server server(5000);
    server.start();
    server.receive_message();
    return 0;
}