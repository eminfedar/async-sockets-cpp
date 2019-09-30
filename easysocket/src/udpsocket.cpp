#include <udpsocket.h>

UDPSocket::UDPSocket(
    int socketId
) :
    BaseSocket(UDP, socketId)
{
    std::thread receivingThread(Receive, this);
    receivingThread.detach();
}

void
UDPSocket::SendTo(
    std::string message,
    std::string ipv4,
    uint16_t port
) {
    this->SendTo(message.c_str(), message.length(), ipv4, port);
}

void
UDPSocket::SendTo(
    const char *bytes,
    size_t byteslength,
    std::string ipv4,
    uint16_t port
) {
    sockaddr_in hostAddr;

    if (inet_pton(AF_INET, ipv4.c_str(), &hostAddr.sin_addr) <= 0)
    {
        if (this->onError)
            this->onError("Invalid IP address. (Required: xxx.yyy.zzz.www)");
        perror("inet_pton");
    }

    hostAddr.sin_port = htons(port);
    hostAddr.sin_family = AF_INET;

    if (sendto(this->sock, bytes, byteslength, 0, (sockaddr *)&hostAddr, sizeof(hostAddr)) < 0)
    {
        if (this->onError)
            this->onError("Error: Cannot send message to the address.");
        perror("sendto");
    }
}

void
UDPSocket::Receive(UDPSocket *udpSocket)
{
    sockaddr_in hostAddr;
    socklen_t hostAddrSize = sizeof(hostAddr);

    char tempBuffer[BUFFER_SIZE];

    while (true)
    {
        int messageLength = recvfrom(udpSocket->sock, tempBuffer, BUFFER_SIZE, 0, (sockaddr *)&hostAddr, &hostAddrSize);
        if (messageLength < 0)
        {
            if (udpSocket->onError)
                udpSocket->onError("Error: Socket receive error.");
            perror("recvfrom");
        }
        else
        {
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength), ipToString(hostAddr), ntohs(hostAddr.sin_port));
        }
    }
}
