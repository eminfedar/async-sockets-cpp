CC		:= g++
CFLAGS	:= --std=c++11 -Wall -Wextra -Werror=conversion
LIBS	:= -lpthread
INC		:= ../async-sockets/include
RM		:= rm

.PHONY: all clean

all: tcp-client tcp-server udp-client udp-server

tcp-client: tcp-client.cpp $(INC)/tcpsocket.hpp
	$(CC) $(CFLAGS) $< -I$(INC) $(LIBS) -o $@

tcp-server: tcp-server.cpp $(INC)/tcpserver.hpp
	$(CC) $(CFLAGS) $< -I$(INC) $(LIBS) -o $@

udp-client: udp-client.cpp $(INC)/udpsocket.hpp
	$(CC) $(CFLAGS) $< -I$(INC) $(LIBS) -o $@

udp-server: udp-server.cpp $(INC)/udpserver.hpp
	$(CC) $(CFLAGS) $< -I$(INC) $(LIBS) -o $@

clean:
	$(RM) tcp-client
	$(RM) tcp-server
	$(RM) udp-client
	$(RM) udp-server
