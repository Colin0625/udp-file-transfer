#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"
#include "utility/Queue.hpp"
#include "transfer/SessionState.hpp"


class ClientSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;
    SessionState state_;

public:
    ClientSession();
    void start();
    void stop();
    ssize_t send(Packet p, SocketAddress addr);
    Packet front();
    void manage_packet();
};