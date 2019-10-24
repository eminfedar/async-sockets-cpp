#include <udpsocket.h>

UDPSocket::UDPSocket(std::function<void(int, std::string)> onError, int socketId) : BaseSocket(onError, UDP, socketId)
{
    std::thread receivingThread(Receive, this);
    receivingThread.detach();
}

void UDPSocket::SendTo(std::string message, std::string ipv4, uint16_t port, std::function<void(int, std::string)> onError)
{
    this->SendTo(message.c_str(), message.length(), ipv4, port, onError);
}

void UDPSocket::SendTo(const char *bytes, size_t byteslength, std::string ipv4, uint16_t port, std::function<void(int, std::string)> onError)
{
    sockaddr_in hostAddr;

    if (inet_pton(AF_INET, ipv4.c_str(), &hostAddr.sin_addr) <= 0)
    {
        onError(errno, "Invalid IP address.");
        return;
    }

    hostAddr.sin_port = htons(port);
    hostAddr.sin_family = AF_INET;

    if (sendto(this->sock, bytes, byteslength, 0, (sockaddr *)&hostAddr, sizeof(hostAddr)) < 0)
    {
        onError(errno, "Cannot send message to the address.");
        return;
    }
}

void UDPSocket::Receive(UDPSocket *udpSocket)
{
    sockaddr_in hostAddr;
    socklen_t hostAddrSize = sizeof(hostAddr);

    char tempBuffer[BUFFER_SIZE];

    while (true)
    {
        int messageLength = recvfrom(udpSocket->sock, tempBuffer, BUFFER_SIZE, 0, (sockaddr *)&hostAddr, &hostAddrSize);
        if (messageLength < 0)
        {
            perror("recvfrom");
            return;
        }
        else
        {
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength), ipToString(hostAddr), ntohs(hostAddr.sin_port));
        }
    }
}
