#include <udpsocket.h>
#include <string.h>

UDPSocket::UDPSocket(bool useConnect, std::function<void(int, std::string)> onError, int socketId) : BaseSocket(onError, UDP, socketId)
{
    if (!useConnect)
    {
        std::thread recvfromThread(ReceiveFrom, this);
        recvfromThread.detach();
    }
    else
    {
        std::thread recvThread(Receive, this);
        recvThread.detach();
    }
}

void UDPSocket::SendTo(std::string message, std::string host, uint16_t port, std::function<void(int, std::string)> onError)
{
    this->SendTo(message.c_str(), message.length(), host, port, onError);
}

void UDPSocket::SendTo(const char *bytes, size_t byteslength, std::string host, uint16_t port, std::function<void(int, std::string)> onError)
{
    sockaddr_in hostAddr;

    struct addrinfo hints, *res, *it;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    if ((status = getaddrinfo(host.c_str(), NULL, &hints, &res)) != 0)
    {
        onError(errno, "Invalid address." + std::string(gai_strerror(status)));
        return;
    }

    for (it = res; it != NULL; it = it->ai_next)
    {
        if (it->ai_family == AF_INET)
        { // IPv4
            memcpy((void *)(&hostAddr), (void *)it->ai_addr, sizeof(sockaddr_in));
            break; // for now, just get first ip (ipv4).
        }
    }

    freeaddrinfo(res);

    hostAddr.sin_port = htons(port);
    hostAddr.sin_family = AF_INET;

    if (sendto(this->sock, bytes, byteslength, 0, (sockaddr *)&hostAddr, sizeof(hostAddr)) < 0)
    {
        onError(errno, "Cannot send message to the address.");
        return;
    }
}

int UDPSocket::Send(std::string message)
{
    return this->Send(message.c_str(), message.length());
}
int UDPSocket::Send(const char *bytes, size_t byteslength)
{
    if (this->isClosed)
        return -1;

    int sent = 0;
    if ((sent = send(this->sock, bytes, byteslength, 0)) < 0)
    {
        perror("send");
    }
    return sent;
}

void UDPSocket::Connect(std::string host, uint16_t port, std::function<void(int, std::string)> onError)
{
    struct addrinfo hints, *res, *it;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int status;
    if ((status = getaddrinfo(host.c_str(), NULL, &hints, &res)) != 0)
    {
        onError(errno, "Invalid address." + std::string(gai_strerror(status)));
        return;
    }

    for (it = res; it != NULL; it = it->ai_next)
    {
        if (it->ai_family == AF_INET)
        { // IPv4
            memcpy((void *)(&this->address), (void *)it->ai_addr, sizeof(sockaddr_in));
            break; // for now, just get first ip (ipv4).
        }
    }

    freeaddrinfo(res);

    this->Connect((uint32_t)this->address.sin_addr.s_addr, port, onError);
}
void UDPSocket::Connect(uint32_t ipv4, uint16_t port, std::function<void(int, std::string)> onError)
{
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = ipv4;

    // Try to connect.
    if (connect(this->sock, (const sockaddr *)&this->address, sizeof(sockaddr_in)) < 0)
    {
        onError(errno, "Connection failed to the host.");
        return;
    }
}

void UDPSocket::Receive(UDPSocket *udpSocket)
{
    char tempBuffer[udpSocket->BUFFER_SIZE];

    while (true)
    {
        int messageLength;
        messageLength = recv(udpSocket->sock, tempBuffer, udpSocket->BUFFER_SIZE, 0);

        if (messageLength < 0)
        {
            perror("recvfrom");
            return;
        }
        else
        {
            tempBuffer[messageLength] = '\0';
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength), ipToString(udpSocket->address), ntohs(udpSocket->address.sin_port));

            if (udpSocket->onRawMessageReceived)
                udpSocket->onRawMessageReceived(tempBuffer, messageLength, ipToString(udpSocket->address), ntohs(udpSocket->address.sin_port));
        }
    }
}

void UDPSocket::ReceiveFrom(UDPSocket *udpSocket)
{
    sockaddr_in hostAddr;
    socklen_t hostAddrSize = sizeof(hostAddr);

    char tempBuffer[udpSocket->BUFFER_SIZE];

    while (true)
    {
        int messageLength;
        messageLength = recvfrom(udpSocket->sock, tempBuffer, udpSocket->BUFFER_SIZE, 0, (sockaddr *)&hostAddr, &hostAddrSize);

        if (messageLength < 0)
        {
            perror("recvfrom");
            return;
        }
        else
        {
            tempBuffer[messageLength] = '\0';
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength), ipToString(hostAddr), ntohs(hostAddr.sin_port));

            if (udpSocket->onRawMessageReceived)
                udpSocket->onRawMessageReceived(tempBuffer, messageLength, ipToString(hostAddr), ntohs(hostAddr.sin_port));
        }
    }
}
