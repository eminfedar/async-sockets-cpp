#ifndef FDR_TCPSERVER_H
#define FDR_TCPSERVER_H

#include <DllHelper.h>

#include <tcpsocket.h>
#include <string>
#include <functional>
#include <thread>

class EASYSOCKET_API TCPServer : public BaseSocket
{
public:
    // Event Listeners:
    std::function<void(TCPSocket *)> onNewConnection;

    TCPServer(FDR_ON_ERROR);

    // Binding the server.
    void Bind(int port, FDR_ON_ERROR);
    void Bind(const char *address, uint16_t port, FDR_ON_ERROR);

    // Start listening the server.
    void Listen(FDR_ON_ERROR);

private:
    static void Accept(TCPServer *server, FDR_ON_ERROR);
};

#endif
