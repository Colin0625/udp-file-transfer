#include <thread>
#include <filesystem>

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
    void set_server_address(const SocketAddress& addr);
    void start();
    void stop();
    ssize_t send(Packet p, SocketAddress addr);
    Packet front();
    void manage_packet();
    void handle_off(Packet& packet);
    void handle_idle(Packet& packet);
    void handle_connecting(Packet& packet);
    void handle_connection(Packet& packet);
    void handle_metatransfer(Packet& packet);
    void handle_transferring(Packet& packet);
    void handle_closing(Packet& packet);

    void connect_to_server();
    void request_file(const std::filesystem::path& file_path);
};
