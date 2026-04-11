#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"


class ClientSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;
    std::thread thread_;



public:
    ClientSession();
    std::thread start_listening();
    int stop_listening();

};