#include <thread>

#include "transfer/Endpoint.hpp"
#include "protocol/Packet.hpp"
#include "utility/Queue.hpp"


class ClientSession {
private:
    Endpoint endpoint_;
    SocketAddress server_address_;
    std::thread thread_;



public:
    ClientSession();
    void start_listening();
    int stop_listening();
    Queue<Packet>* get_queue();
    ssize_t send(Packet p, SocketAddress addr);
};