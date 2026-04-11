#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"


class ServerSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;
    std::thread thread_;



public:
    ServerSession();
    void bind_server(int port);
    std::thread start_listening();
    int stop_listening();

};