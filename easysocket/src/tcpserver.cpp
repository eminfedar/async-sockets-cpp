#include <tcpserver.h>

TCPServer::TCPServer(std::function<void(int, std::string)> onError) : BaseSocket(onError, TCP)
{

}

void TCPServer::Bind(int port, std::function<void(int, std::string)> onError)
{
    this->Bind("0.0.0.0", port, onError);
}

void TCPServer::Bind(const char* address, uint16_t port, std::function<void(int, std::string)> onError)
{
    if (inet_pton(AF_INET, address, &this->address.sin_addr) <= 0)
    {
        onError(errno, "Invalid address. Address type not supported.");
        return;
    }

    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);

    if (bind(this->sock, (const sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        onError(errno, "Cannot bind the socket.");
        return;
    }
}

void TCPServer::Listen(std::function<void(int, std::string)> onError)
{
    if (listen(this->sock, 10) < 0)
    {
        onError(errno, "Error: Server can't listen the socket.");
        return;
    }

    std::thread acceptThread(Accept, this, onError);
    acceptThread.detach();
}

void TCPServer::Accept(TCPServer *server, std::function<void(int, std::string)> onError)
{
    sockaddr_in newSocketInfo;
    socklen_t newSocketInfoLength = sizeof(newSocketInfo);

    int newSock;
    while (true)
    {
        if ((newSock = accept(server->sock, (sockaddr *)&newSocketInfo, &newSocketInfoLength)) < 0)
        {
            onError(errno, "Error while accepting a new connection.");
            return;
        }

        TCPSocket *newSocket = new TCPSocket([](int e, std::string er){}, newSock);
        newSocket->setAddressStruct(newSocketInfo);

        server->onNewConnection(newSocket);
        newSocket->Listen();
    }
}
