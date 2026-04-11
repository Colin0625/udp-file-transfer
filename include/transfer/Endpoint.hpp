


#include "net/SocketAddress.hpp"
#include "net/UdpSocket.hpp"
#include "utility/Queue.hpp"
#include "protocol/Packet.hpp"
#include "protocol/PacketHeader.hpp"


class Endpoint {
private:
    UdpSocket socket_;
    Queue<Packet> queue_;
    bool running_;



public:
    Endpoint();
    void bind_endpoint(const SocketAddress& addr);
    void packet_receiver();
    ssize_t send_packet(const Packet& packet, const SocketAddress& addr);
    int close_receiver();
    


};