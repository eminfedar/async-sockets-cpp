#pragma once

#include "tcpsocket.hpp"
#include <thread>

class TCPServer : public BaseSocket
{
public:
    // Event Listeners:
    std::function<void(TCPSocket*)> onNewConnection = [](TCPSocket* sock){FDR_UNUSED(sock)};

    explicit TCPServer(FDR_ON_ERROR): BaseSocket(onError, SocketType::TCP)
    {
        int opt = 1;
        setsockopt(this->sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
        setsockopt(this->sock,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(int));
    }

    // Bind the custom address & port of the server.
    void Bind(const char* address, uint16_t port, FDR_ON_ERROR)
    {
        int status = inet_pton(AF_INET, address, &this->address.sin_addr);
        switch (status) {
            case -1:
                onError(errno, "Invalid address. Address type not supported.");
                return;
            case 0:
                onError(errno, "AF_INET is not supported. Please send message to developer.");
                return;
            default:
                break;
        }

        this->address.sin_family = AF_INET;
        this->address.sin_port = htons(port);

        if (bind(this->sock, (const sockaddr*)&this->address, sizeof(this->address)) == -1)
        {
            onError(errno, "Cannot bind the socket.");
            return;
        }
    }
    // Bind the address(0.0.0.0) & port of the server.
    void Bind(uint16_t port, FDR_ON_ERROR) { this->Bind("0.0.0.0", port, onError); }

    // Start listening incoming connections.
    void Listen(FDR_ON_ERROR)
    {
        if (listen(this->sock, 20) == -1)
        {
            onError(errno, "Error: Server can't listen the socket.");
            return;
        }

        std::thread t(Accept, this, onError);
        t.detach();
    }

private:
    static void Accept(TCPServer* server, FDR_ON_ERROR)
    {
        sockaddr_in newSocketInfo;
        socklen_t newSocketInfoLength = sizeof(newSocketInfo);

        int newSocketFileDescriptor = -1;
        while (true)
        {
            newSocketFileDescriptor = accept(server->sock, (sockaddr*)&newSocketInfo, &newSocketInfoLength);
            if (newSocketFileDescriptor == -1)
            {
                if (errno == EBADF || errno == EINVAL) return;

                onError(errno, "Error while accepting a new connection.");

                return;
            }

            TCPSocket* newSocket = new TCPSocket(onError, newSocketFileDescriptor);
            newSocket->deleteAfterClosed = true;
            newSocket->setAddressStruct(newSocketInfo);

            server->onNewConnection(newSocket);
            newSocket->Listen();
        }
    }
};