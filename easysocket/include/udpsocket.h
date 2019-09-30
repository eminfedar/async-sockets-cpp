#ifndef FDR_UDPSOCKET_H
#define FDR_UDPSOCKET_H

#include <DllHelper.h>

#include <basesocket.h>
#include <string>
#include <functional>
#include <thread>

#define BUFFER_SIZE 0xFFFF

class EASYSOCKET_API UDPSocket : public BaseSocket
{
public:
    std::function<void(std::string, std::string, std::uint16_t)> onMessageReceived;

    UDPSocket(int socketId = -1);

    void SendTo(std::string message, std::string ipv4, uint16_t port);
    void SendTo(const char *bytes, size_t byteslength, std::string ipv4, uint16_t port);

private:
    static void Receive(UDPSocket *udpSocket);
};

#endif
