build:
	g++ tcp-client.cpp --std=c++11 -o tcp-client.exe -lpthread
	g++ udp-client.cpp --std=c++11 -o udp-client.exe -lpthread
	g++ tcp-server.cpp --std=c++11 -o tcp-server.exe -lpthread
	g++ udp-server.cpp --std=c++11 -o udp-server.exe -lpthread

clean:
	rm tcp-client.exe
	rm tcp-server.exe
	rm udp-client.exe
	rm udp-server.exe