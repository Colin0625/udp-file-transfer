CXX = g++-12
FLAGS = -std=c++20 -Iinclude


all: server_main client_main

Endpoint.o: src/Endpoint.cpp include/Endpoint.hpp
	$(CXX) $(FLAGS) -c src/Endpoint.cpp -o Endpoint.o

Server.o: src/Server.cpp include/Server.hpp
	$(CXX) $(FLAGS) -c src/Server.cpp -o Server.o

Client.o: src/Client.cpp include/Client.hpp
	$(CXX) $(FLAGS) -c src/Client.cpp -o Client.o

server_main: Endpoint.o Server.o src/server_main.cpp
	$(CXX) $(FLAGS) Endpoint.o Server.o src/server_main.cpp -o server.out

client_main: Endpoint.o Client.o src/client_main.cpp
	$(CXX) $(FLAGS) Endpoint.o Client.o src/client_main.cpp -o client.out




clean:
	rm -f *.out *.o