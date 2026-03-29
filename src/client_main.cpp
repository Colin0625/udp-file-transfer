#include "Client.hpp"

int main() {
    Client client;
    std::string file = "cpp.png";
    client.connect_to_server(5000, "127.0.0.1");
    client.request_file(file);
    return 1;
}