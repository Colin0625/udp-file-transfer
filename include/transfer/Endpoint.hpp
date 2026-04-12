#include <atomic>
#include <thread>

#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "utility/Queue.hpp"
#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"


class Endpoint {
private:
    UdpSocket socket_;
    Queue<Packet> queue_;
    std::atomic<bool> running_;
    std::thread thread_;
    SocketAddress server_address_;


    void reception_loop();
public:
    Endpoint();
    void bind_endpoint(const SocketAddress& addr);
    ssize_t send_packet(const Packet& packet, const SocketAddress& addr);
    void start_receiver();
    int stop_receiver();
    Packet* get_front_packet();

};