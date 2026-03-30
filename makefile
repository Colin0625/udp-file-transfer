CXX = g++-13
FLAGS = -std=c++20 -Iinclude

all: server client

SocketAddress.o: src/SocketAddress.cpp include/SocketAddress.hpp
	$(CXX) $(FLAGS) -c src/SocketAddress.cpp -o SocketAddress.o

UdpSocket.o: src/UdpSocket.cpp include/UdpSocket.hpp SocketAddress.o
	$(CXX) $(FLAGS) -c src/UdpSocket.cpp -o UdpSocket.o

server: SocketAddress.o UdpSocket.o src/server.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o src/server.cpp -o server.out

client: SocketAddress.o UdpSocket.o src/client.cpp
	$(CXX) $(FLAGS) SocketAddress.o UdpSocket.o src/client.cpp -o client.out


clean:
	rm -f *.o *.out