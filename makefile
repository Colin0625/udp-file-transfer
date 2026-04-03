CXX = g++-13
FLAGS = -std=c++20 -Iinclude

all: server client

SocketAddress.o: src/net/SocketAddress.cpp include/net/SocketAddress.hpp
	$(CXX) $(FLAGS) -c src/net/SocketAddress.cpp -o SocketAddress.o

UdpSocket.o: src/net/UdpSocket.cpp include/net/UdpSocket.hpp SocketAddress.o
	$(CXX) $(FLAGS) -c src/net/UdpSocket.cpp -o UdpSocket.o

Packet.o: src/protocol/Packet.cpp include/protocol/Packet.hpp include/protocol/PacketHeader.hpp include/protocol/MessageType.hpp
	$(CXX) $(FLAGS) -c src/protocol/Packet.cpp -o Packet.o

FilePackager.o: src/packaging/FilePackager.cpp include/packaging/FilePackager.hpp Packet.o include/protocol/MessageType.hpp include/protocol/PacketHeader.hpp
	$(CXX) $(FLAGS) -c src/packaging/FilePackager.cpp -o FilePackager.o

FileReassembler.o: src/packaging/FileReassembler.cpp include/packaging/FileReassembler.hpp Packet.o include/protocol/MessageType.hpp include/protocol/PacketHeader.hpp FilePackager.o
	$(CXX) $(FLAGS) -c src/packaging/FileReassembler.cpp -o FileReassembler.o


server: SocketAddress.o UdpSocket.o Packet.o src/server_main.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o Packet.o src/server_main.cpp -o server.out

client: SocketAddress.o UdpSocket.o Packet.o src/client_main.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o Packet.o src/client_main.cpp -o client.out

test: FilePackager.o Packet.o FileReassembler.o src/FilePackagerTester.cpp
	$(CXX) $(FLAGS) Packet.o FilePackager.o FileReassembler.o src/FilePackagerTester.cpp -o test.out

clean:
	rm -f *.o *.out