#include <tcpserver.h>

TCPServer::TCPServer() :
    BaseSocket(TCP)
{
    
}

void
TCPServer::Bind(
    int port
) {
    this->Bind("0.0.0.0", port);
}

void
TCPServer::Bind(
    const char* address,
    uint16_t port
) {
    if (inet_pton(AF_INET, address, &this->address.sin_addr) <= 0)
    {
        if (this->onError)
        {
            this->onError("Error: Invalid address. Address type not supported.");
            return;
        }
    }

    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);

    if (bind(this->sock, (const sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        if (this->onError)
            this->onError("Error: Cannot bind the socket.");
        perror("bind");

        return;
    }
}

void
TCPServer::Listen()
{
    if (listen(this->sock, 10) < 0)
    {
        if (this->onError)
        {
            this->onError("Error: Server can't listen the socket.");
            return;
        }
    }

    std::thread acceptThread(Accept, this);
    acceptThread.detach();
}

void
TCPServer::Accept(TCPServer *server)
{
    sockaddr_in newSocketInfo;
    socklen_t newSocketInfoLength = sizeof(newSocketInfo);

    int newSock;
    while (true)
    {
        if ((newSock = accept(server->sock, (sockaddr *)&newSocketInfo, &newSocketInfoLength)) < 0)
        {
            if (server->onError)
            {
                server->onError("Error: Cannot accept a new connection.");
                return;
            }
        }

        Socket *newSocket = new Socket(newSock);
        newSocket->setAddressStruct(newSocketInfo);

        server->onNewConnection(newSocket);
        newSocket->Listen();
    }
}
