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

#define FDR_UNUSED(expr){ (void)(expr); } 
#define FDR_ON_ERROR std::function<void(int, std::string)> onError = [](int errorCode, std::string errorMessage){FDR_UNUSED(errorCode); FDR_UNUSED(errorMessage)}

class BaseSocket
{
public:
    enum SocketType
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };
    sockaddr_in address;
    constexpr static uint16_t BUFFER_SIZE = 0x1000; // 4096 bytes

    void Close() {
        shutdown(this->sock, SHUT_RDWR);
        close(this->sock);
    }

    std::string remoteAddress() const { return ipToString(this->address); }
    int remotePort() const { return ntohs(this->address.sin_port); }
    int fileDescriptor() const { return this->sock; }

protected:
    int sock = 0;

    // Get std::string value of the IP from a `sockaddr_in` address struct
    static std::string ipToString(const sockaddr_in& addr)
    {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

        return std::string(ip);
    }  

    BaseSocket(FDR_ON_ERROR, SocketType sockType = TCP, int socketId = -1)
    {
        if (socketId == -1)
        {
            this->sock = socket(AF_INET, sockType, 0);
            
            if ( this->sock == -1 )
            {
                onError(errno, "Socket creating error.");
            }
        }
        else
        {
            this->sock = socketId;
        }
    }
    virtual ~BaseSocket(){}
};