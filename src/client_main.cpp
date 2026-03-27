#include "Client.hpp"

int main() {
    Client client;
    client.connect_to_server(5000, "127.0.0.1");
    return 1;
}