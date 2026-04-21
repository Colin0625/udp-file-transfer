#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"
#include "transfer/SessionState.hpp"


class ServerSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;
    SessionState state_;

public:
    ServerSession();
    void bind_server(int port);
    void start();
    void stop();
    Packet front();
    void handle_packet();
};