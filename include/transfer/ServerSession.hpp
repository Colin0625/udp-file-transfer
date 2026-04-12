#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"


class ServerSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;

public:
    ServerSession();
    void bind_server(int port);
    void start();
    void stop();
    Packet* front();
};