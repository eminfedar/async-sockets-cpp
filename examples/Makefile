CC		:= g++
CFLAGS	:= --std=c++11
LIBS	:= -lpthread
INC		:= -I../async-sockets/include
RM		:= rm

all: tcp-client tcp-server udp-client udp-server

tcp-client:
	$(CC) $(CFLAGS) tcp-client.cpp $(INC) $(LIBS) -o tcp-client

tcp-server:
	$(CC) $(CFLAGS) tcp-server.cpp $(INC) $(LIBS) -o tcp-server

udp-client:
	$(CC) $(CFLAGS) udp-client.cpp $(INC) $(LIBS) -o udp-client

udp-server:
	$(CC) $(CFLAGS) udp-server.cpp $(INC) $(LIBS) -o udp-server

clean:
	$(RM) tcp-client
	$(RM) tcp-server
	$(RM) udp-client
	$(RM) udp-server