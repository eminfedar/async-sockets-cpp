#ifndef FDR_BASESOCKET_H
#define FDR_BASESOCKET_H

#include <DllHelper.h>

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

class EASYSOCKET_API BaseSocket
{
protected:
    int sock = 0;

    static std::string ipToString(sockaddr_in addr);

public:
    enum EASYSOCKET_API SocketType
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };

    sockaddr_in address;
    bool isClosed = false;

    std::function<void(std::string)> onError;

    BaseSocket(SocketType sockType = TCP, int socketId = -1);

    void Close();

    std::string remoteAddress();
    int remotePort();
};

#endif
