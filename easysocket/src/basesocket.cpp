#include <basesocket.h>

std::string BaseSocket::ipToString(sockaddr_in addr)
{
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

    return std::string(ip);
}

BaseSocket::BaseSocket(std::function<void(int, std::string)> onError, SocketType sockType, int socketId) : sock(socketId)
{
    if (socketId < 0)
    {
        if ((this->sock = socket(AF_INET, sockType, 0)) < 0)
        {
            onError(errno, "Socket creating error.");
        }
    }
}

void BaseSocket::Close()
{
    isClosed = true;
    close(this->sock);
}

std::string BaseSocket::remoteAddress()
{
    return ipToString(this->address);
}

int BaseSocket::remotePort()
{
    return ntohs(this->address.sin_port);
}
