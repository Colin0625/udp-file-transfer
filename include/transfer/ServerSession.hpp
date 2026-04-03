#include "net/UdpSocket.hpp"
#include "packaging/FilePackager.hpp"


class ServerSession {
private:
    UdpSocket socket_;
    FilePackager file_packager_;




public:
    ServerSession();
    void accept_connection();




};