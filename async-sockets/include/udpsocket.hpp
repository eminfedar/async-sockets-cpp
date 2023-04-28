#pragma once

#include "basesocket.hpp"
#include <string.h>
#include <thread>

class UDPSocket : public BaseSocket
{
public:
    std::function<void(std::string, std::string, std::uint16_t)> onMessageReceived;
    std::function<void(const char*, ssize_t, std::string, std::uint16_t)> onRawMessageReceived;

    explicit UDPSocket(bool useConnect = false, FDR_ON_ERROR, int socketId = -1): BaseSocket(onError, SocketType::UDP, socketId)
    {
        if (useConnect)
        {
            
            std::thread t(Receive, this); // usage with Connect()
            t.detach();
        }
        else
        {
            std::thread t(ReceiveFrom, this);
            t.detach();
        }
    }
    
    // Send raw bytes to a spesific `host` & `port` with no connection
    ssize_t SendTo(const char* bytes, size_t byteslength, const char* host, uint16_t port, FDR_ON_ERROR)
    {
        sockaddr_in hostAddr;

        struct addrinfo hints, *res, *it;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;

        int status;
        if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0)
        {
            onError(errno, "Invalid address." + std::string(gai_strerror(status)));
            return -1;
        }

        for (it = res; it != NULL; it = it->ai_next)
        {
            if (it->ai_family == AF_INET)
            { // IPv4
                memcpy((void* )(&hostAddr), (void* )it->ai_addr, sizeof(sockaddr_in));
                break; // for now, just get first ip (ipv4).
            }
        }

        freeaddrinfo(res);

        hostAddr.sin_port = htons(port);
        hostAddr.sin_family = AF_INET;
        
        ssize_t sent_length = sendto(this->sock, bytes, byteslength, 0, (sockaddr*)&hostAddr, sizeof(hostAddr));
        if (sent_length == -1)
        {
            onError(errno, "Cannot send message to the address.");
            return -1;
        }

        return sent_length;
    }
    // Send raw bytes to a spesific `host` & `port` with no connection
    ssize_t SendTo(const char* bytes, size_t byteslength, const std::string& host, uint16_t port, FDR_ON_ERROR)
    {
        return this->SendTo(bytes, byteslength, host.c_str(), port, onError);
    }
    // Send std::string to a spesific `host` & `port` with no connection
    ssize_t SendTo(const std::string& message, const char* host, uint16_t port, FDR_ON_ERROR)
    {
        return this->SendTo(message.c_str(), message.length(), host, port, onError);
    }
    // Send std::string to a spesific `host` & `port` with no connection
    ssize_t SendTo(const std::string& message, const std::string& host, uint16_t port, FDR_ON_ERROR)
    {
        return this->SendTo(message.c_str(), message.length(), host.c_str(), port, onError);
    }

    // Send raw bytes to the `Connect()`ed server.
    ssize_t Send(const char* bytes, size_t byteslength) { return send(this->sock, bytes, byteslength, 0); }
    // Send std::string to the `Connect()`ed server.
    ssize_t Send(const std::string& message) { return this->Send(message.c_str(), message.length()); }

    // Connect to a server with raw `uint32_t ipv4` and `uint16_t port` values.
    void Connect(uint32_t ipv4, uint16_t port, FDR_ON_ERROR)
    {
        this->address.sin_family = AF_INET;
        this->address.sin_port = htons(port);
        this->address.sin_addr.s_addr = ipv4;

        // Try to connect.
        int status = connect(this->sock, (const sockaddr* )&this->address, sizeof(sockaddr_in));
        if (status == -1)
        {
            onError(errno, "Connection failed to the host.");
            return;
        }
    }
    // Connect to a server with `host` address and `port` values.
    void Connect(const char* host, uint16_t port, FDR_ON_ERROR)
    {
        struct addrinfo hints, *res, *it;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;

        int status = getaddrinfo(host, NULL, &hints, &res);
        if (status != 0)
        {
            onError(errno, "Invalid address." + std::string(gai_strerror(status)));
            return;
        }

        for (it = res; it != NULL; it = it->ai_next)
        {
            if (it->ai_family == AF_INET)
            { // IPv4
                memcpy((void* )(&this->address), (void*)it->ai_addr, sizeof(sockaddr_in));
                break; // for now, just get first ip (ipv4).
            }
        }

        freeaddrinfo(res);

        this->Connect((uint32_t)this->address.sin_addr.s_addr, port, onError);
    }
    // Connect to a server with `host` address and `port` values.
    void Connect(const std::string& host, uint16_t port, FDR_ON_ERROR) { this->Connect(host.c_str(), port, onError); }

private:
    static void Receive(UDPSocket* udpSocket)
    {
        char tempBuffer[UDPSocket::BUFFER_SIZE];
        ssize_t messageLength;

        while ((messageLength = recv(udpSocket->sock, tempBuffer, UDPSocket::BUFFER_SIZE, 0)) != -1)
        {
            tempBuffer[messageLength] = '\0';
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer, messageLength), ipToString(udpSocket->address), ntohs(udpSocket->address.sin_port));

            if (udpSocket->onRawMessageReceived)
                udpSocket->onRawMessageReceived(tempBuffer, messageLength, ipToString(udpSocket->address), ntohs(udpSocket->address.sin_port));
        }
    }

    static void ReceiveFrom(UDPSocket* udpSocket)
    {
        sockaddr_in hostAddr;
        socklen_t hostAddrSize = sizeof(hostAddr);

        char tempBuffer[UDPSocket::BUFFER_SIZE];
        ssize_t messageLength;

        while ((messageLength = recvfrom(udpSocket->sock, tempBuffer, UDPSocket::BUFFER_SIZE, 0, (sockaddr* )&hostAddr, &hostAddrSize)) != -1)
        {
            tempBuffer[messageLength] = '\0';
            if (udpSocket->onMessageReceived)
                udpSocket->onMessageReceived(std::string(tempBuffer, messageLength), ipToString(hostAddr), ntohs(hostAddr.sin_port));

            if (udpSocket->onRawMessageReceived)
                udpSocket->onRawMessageReceived(tempBuffer, messageLength, ipToString(hostAddr), ntohs(hostAddr.sin_port));
        }
    }
};