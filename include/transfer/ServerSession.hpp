

#include "net/UdpSocket.hpp"
#include "packaging/FilePackager.hpp"


class ServerSession {
private:
    UdpSocket socket_;
    SocketAddress server_address_;
    FilePackager file_packager_;
    SocketAddress current_client_address_;

public:
    ServerSession();
    int start_server(uint16_t port);
    int accept_connection();

};