#include <thread>

#include "transfer/ClientSession.hpp"

ClientSession::ClientSession() : endpoint_{}, server_address_{} {}

std::thread ClientSession::start_listening() {
    return std::thread(&Endpoint::packet_receiver, &this->endpoint_);
}

int ClientSession::stop_listening() {
    return endpoint_.close_receiver();
}