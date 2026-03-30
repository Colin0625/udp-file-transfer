CXX = g++-13
FLAGS = -std=c++20 -Iinclude

all: server client

SocketAddress.o: src/net/SocketAddress.cpp include/net/SocketAddress.hpp
	$(CXX) $(FLAGS) -c src/net/SocketAddress.cpp -o SocketAddress.o

UdpSocket.o: src/net/UdpSocket.cpp include/net/UdpSocket.hpp SocketAddress.o
	$(CXX) $(FLAGS) -c src/net/UdpSocket.cpp -o UdpSocket.o

server: SocketAddress.o UdpSocket.o src/server_main.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o src/server_main.cpp -o server.out

client: SocketAddress.o UdpSocket.o src/client_main.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o src/client_main.cpp -o client.out


clean:
	rm -f *.o *.out