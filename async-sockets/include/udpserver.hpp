#pragma once

#include "udpsocket.hpp"
#include <thread>

template <uint16_t BUFFER_SIZE = AS_DEFAULT_BUFFER_SIZE>
class UDPServer : public UDPSocket<BUFFER_SIZE>
{
public:
    // Bind the custom address & port of the server.
    void Bind(const char* address, std::uint16_t port, FDR_ON_ERROR)
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

        status = bind(this->sock, (const sockaddr*)&this->address, sizeof(this->address));
        if (status == -1)
        {
            onError(errno, "Cannot bind the socket.");
            return;
        }
    }
    
    // Bind the address(0.0.0.0) & port of the server.
    void Bind(uint16_t port, FDR_ON_ERROR)
    {
        this->Bind("0.0.0.0", port, onError);
    }

    // Enable or disable the SO_BROADCAST flag
    void setBroadcast(bool value, FDR_ON_ERROR)
    {
        int broadcast = static_cast<int>(value);
        int status = setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);
        if (status == -1)
        {
            onError(errno, "setsockopt(SO_BROADCAST) failed.");
            return;
        }
    }
};