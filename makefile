CXX = g++-13
FLAGS = -std=c++20 -Iinclude

all: server client

SocketAddress.o: src/net/SocketAddress.cpp include/net/SocketAddress.hpp
	$(CXX) $(FLAGS) -c src/net/SocketAddress.cpp -o SocketAddress.o

UdpSocket.o: src/net/UdpSocket.cpp include/net/UdpSocket.hpp SocketAddress.o
	$(CXX) $(FLAGS) -c src/net/UdpSocket.cpp -o UdpSocket.o

Packet.o: src/protocol/Packet.cpp include/protocol/Packet.hpp include/protocol/PacketHeader.hpp include/protocol/MessageType.hpp
	$(CXX) $(FLAGS) -c src/protocol/Packet.cpp -o Packet.o

server: SocketAddress.o UdpSocket.o Packet.o src/server_main.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o Packet.o src/server_main.cpp -o server.out

client: SocketAddress.o UdpSocket.o Packet.o src/client_main.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o Packet.o src/client_main.cpp -o client.out

test: include/transfer/FilePackager.hpp src/transfer/FilePackager.cpp src/FilePackagerTester.cpp include/protocol/PacketHeader.hpp include/protocol/MessageType.hpp
	$(CXX) $(FLAGS) src/transfer/FilePackager.cpp src/FilePackagerTester.cpp -o test.out


clean:
	rm -f *.o *.out