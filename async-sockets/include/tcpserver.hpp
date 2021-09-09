#pragma once

#include "tcpsocket.hpp"
#include <thread>

class TCPServer : public BaseSocket
{
public:
    // Event Listeners:
    std::function<void(TCPSocket *)> onNewConnection = [](TCPSocket* sock){FDR_UNUSED(sock)};

    explicit TCPServer(FDR_ON_ERROR): BaseSocket(onError, SocketType::TCP)
    {
        int opt = 1;
        setsockopt(this->sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
        setsockopt(this->sock,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(int));
    }

    // Binding the server.
    void Bind(const char *address, uint16_t port, FDR_ON_ERROR)
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
    void Bind(int port, FDR_ON_ERROR) { this->Bind("0.0.0.0", port, onError); }

    // Start listening the server.
    void Listen(FDR_ON_ERROR)
    {
        if (listen(this->sock, 20) < 0)
        {
            onError(errno, "Error: Server can't listen the socket.");
            return;
        }

        std::thread t(Accept, this, onError);
        t.detach();
    }

    // Overriding Close to add shutdown():
    void Close()
    {
        shutdown(this->sock, SHUT_RDWR);
        
        BaseSocket::Close();
    }

private:
    static void Accept(TCPServer *server, FDR_ON_ERROR)
    {
        sockaddr_in newSocketInfo;
        socklen_t newSocketInfoLength = sizeof(newSocketInfo);

        int newSock;
        while (!server->isClosed)
        {
            while ((newSock = accept(server->sock, (sockaddr *)&newSocketInfo, &newSocketInfoLength)) < 0)
            {
                if (errno == EBADF || errno == EINVAL) return;

                onError(errno, "Error while accepting a new connection.");
                return;
            }

            if (!server->isClosed && newSock >= 0)
            {
                TCPSocket *newSocket = new TCPSocket(onError, newSock);
                newSocket->setAddressStruct(newSocketInfo);

                server->onNewConnection(newSocket);
                newSocket->Listen();
            }
        }
    }
};