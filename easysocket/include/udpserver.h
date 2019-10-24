#ifndef FDR_UDPSERVER_H
#define FDR_UDPSERVER_H

#include <DllHelper.h>

#include <udpsocket.h>
#include <string>

class EASYSOCKET_API UDPServer : public UDPSocket
{
public:
    UDPServer();

    void Bind(int port, FDR_ON_ERROR);
    void Bind(std::string IPv4, std::uint16_t port, FDR_ON_ERROR);
};

#endif
