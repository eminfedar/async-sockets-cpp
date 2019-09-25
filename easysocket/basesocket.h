#ifndef FDR_BASESOCKET_H
#define FDR_BASESOCKET_H

#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#elif _WIN32
#include <winsock32.h>
#endif

#include <string>
#include <functional>

using namespace std;

class BaseSocket
{
protected:
    int sock = 0;

    static string ipToString(sockaddr_in addr)
    {

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

        return string(ip);
    }

public:
    enum SocketType
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };

    sockaddr_in address;
    bool isClosed = false;

    function<void(string)> onError;

    BaseSocket(SocketType sockType = TCP, int socketId = -1) : sock(socketId)
    {
        if (socketId < 0)
        {
            if ((this->sock = socket(AF_INET, sockType, 0)) < 0)
            {
                if (this->onError)
                    this->onError("Socket initialization Error.");

                perror("socket");
            }
        }
    }

    void Close()
    {
        isClosed = true;
        close(this->sock);
    }

    string remoteAddress()
    {
        return ipToString(this->address);
    }

    int remotePort()
    {
        return ntohs(this->address.sin_port);
    }
};

#endif