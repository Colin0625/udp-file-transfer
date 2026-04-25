#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"
#include "transfer/SessionState.hpp"


class ServerSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;
    SocketAddress current_client_address_;
    SessionState state_;

public:
    ServerSession();
    void bind_server(int port);
    void start();
    void stop();
    Packet front();
    void manage_packet();
    void handle_off(Packet& packet);
    void handle_idle(Packet& packet);
    void handle_connecting(Packet& packet);
    void handle_connection(Packet& packet);
    void handle_metatransfer(Packet& packet);
    void handle_transferring(Packet& packet);
    void handle_closing(Packet& packet);
};