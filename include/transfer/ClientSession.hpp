#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"
#include "utility/Queue.hpp"


class ClientSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;

public:
    ClientSession();
    void start();
    void stop();
    ssize_t send(Packet p, SocketAddress addr);
    Packet* front();
};