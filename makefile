FLAGS = -std=c++20 -Iinclude

build:
	g++ $(FLAGS) src/Server.cpp src/Endpoint.cpp -o server.out
	g++ $(FLAGS) src/Client.cpp src/Endpoint.cpp -o client.out

clean:
	rm -f *.out *.o