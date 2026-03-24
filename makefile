FLAGS = -std=c++20 -Iinclude

build:
	g++ $(FLAGS) src/server_main.cpp src/Server.cpp src/Endpoint.cpp -o server.out
	g++ $(FLAGS) src/client_main.cpp src/Client.cpp src/Endpoint.cpp -o client.out

clean:
	rm -f *.out *.o