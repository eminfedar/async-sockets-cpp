#ifndef FDR_TCPSERVER_H
#define FDR_TCPSERVER_H

#include "socket.h"
#include <cstring>
#include <functional>
#include <thread>

using namespace std;

class TCPServer : public BaseSocket
{
public:
    // Event Listeners:
    function<void(Socket *)> onNewConnection;

    TCPServer() : BaseSocket(TCP)
    {
    }
    ~TCPServer()
    {
        cout << "DECONS!";
    }

    // Binding the server.
    void Bind(int port)
    {
        this->Bind("0.0.0.0", port);
    }
    void Bind(const char *address, uint16_t port)
    {
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

    // Start listening the server.
    void Listen()
    {
        if (listen(this->sock, 10) < 0)
        {
            if (this->onError)
            {
                this->onError("Error: Server can't listen the socket.");
                return;
            }
        }

        thread acceptThread(Accept, this);
        acceptThread.detach();
    }

private:
    static void Accept(TCPServer *server)
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
};

#endif