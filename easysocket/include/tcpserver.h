#ifndef FDR_TCPSERVER_H
#define FDR_TCPSERVER_H

#include <DllHelper.h>

#include <socket.h>
#include <string>
#include <functional>
#include <thread>

class EASYSOCKET_API TCPServer : public BaseSocket
{
public:
    // Event Listeners:
    std::function<void(Socket *)> onNewConnection;

    TCPServer();

    // Binding the server.
    void Bind(int port);
    void Bind(const char *address, uint16_t port);

    // Start listening the server.
    void Listen();

private:
    static void Accept(TCPServer *server);
};

#endif
