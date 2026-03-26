#include "Client.hpp"

int main() {
    Client client(5000, "127.0.0.1");
    client.send_message(MessageType::GET);
    return 1;
}