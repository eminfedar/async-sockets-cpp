#pragma once

#if defined(__linux__) || defined(__APPLE__)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#elif _WIN32
#include <winsock32.h>
#endif

#include <string>
#include <functional>
#include <cerrno>
#include <atomic>

#define FDR_UNUSED(expr){ (void)(expr); } 
#define FDR_ON_ERROR std::function<void(int, std::string)> onError = [](int errorCode, std::string errorMessage){FDR_UNUSED(errorCode); FDR_UNUSED(errorMessage)}

class BaseSocket
{
// Definitions
public:
    enum SocketType
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };
    sockaddr_in address;
    constexpr static uint16_t BUFFER_SIZE = 0x1000; // 4096 bytes

protected:
    int sock = 0;
    static std::string ipToString(sockaddr_in addr)
    {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

        return std::string(ip);
    }  

    BaseSocket(FDR_ON_ERROR, SocketType sockType = TCP, int socketId = -1)
    {
        if (socketId < 0)
        {
            if ((this->sock = socket(AF_INET, sockType, 0)) < 0)
            {
                onError(errno, "Socket creating error.");
            }
        }
        else
        {
            this->sock = socketId;
        }
    }

// Methods
public:
    virtual void Close() {
        close(this->sock);
    }

    std::string remoteAddress() {return ipToString(this->address);}
    int remotePort() {return ntohs(this->address.sin_port);}
    int fileDescriptor() const { return this->sock; }
};