#ifndef FDR_SOCKET_H
#define FDR_SOCKET_H

#include <DllHelper.h>

#include <basesocket.h>
#include <string>
#include <functional>
#include <thread>

#define BUFFER_SIZE 0xFFFF

class EASYSOCKET_API Socket : public BaseSocket
{
public:
    // Event Listeners:
    std::function<void(std::string)> onMessageReceived;
    std::function<void()> onSocketClosed;

    Socket(int socketId = -1);

    void Send(std::string message);
    void Send(const char *bytes, size_t byteslength);

    void Connect(std::string ipv4, uint16_t port, std::function<void()> onConnected);
    void Connect(uint32_t IPv4, uint16_t port, std::function<void()> onConnected);

    void Listen();

    void setAddressStruct(sockaddr_in addr);

private:
    static void Receive(Socket *socket);
};

#endif
