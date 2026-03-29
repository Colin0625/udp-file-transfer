#include "Client.hpp"

int main() {
    Client client;
    std::string file = "cpp.png";
    client.connect_to_server(5000, "100.83.112.113");
    client.request_file(file);
    return 1;
}