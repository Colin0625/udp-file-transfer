#include <thread>

#include "transfer/ServerSession.hpp"

ServerSession::ServerSession() : endpoint_{}, server_address_{} {}

void ServerSession::bind_server(int port) {
    SocketAddress addr = SocketAddress::any(port);
    endpoint_.bind_endpoint(addr);
}

void ServerSession::start_listening() {
    std::thread t = std::thread(&Endpoint::packet_receiver, &this->endpoint_);
    thread_ = std::move(t);
}

int ServerSession::stop_listening() {
    int res = endpoint_.close_receiver();
    std::cout << "result: " << res << std::endl;
    thread_.join();
    return res;
}

Queue<Packet>* ServerSession::get_queue() {
    return endpoint_.get();
}