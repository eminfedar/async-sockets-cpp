#ifndef FDR_UDPSERVER_H
#define FDR_UDPSERVER_H

#include <DllHelper.h>

#include <udpsocket.h>
#include <string>

class EASYSOCKET_API UDPServer : public UDPSocket
{
public:
    UDPServer();

    void Bind(int port);
    void Bind(const char *address, std::uint16_t port);
};

#endif
