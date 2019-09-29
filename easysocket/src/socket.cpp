#include <socket.h>

Socket::Socket(
    int socketId
) :
    BaseSocket(TCP, socketId)
{

}

void
Socket::Send(
    std::string message
) {
    this->Send(message.c_str(), message.length());
}

void
Socket::Send(
    const char* bytes,
    size_t byteslength
) {
    if (this->isClosed)
    {
        if (this->onError)
            this->onError("Error: Socket is closed.");

        return;
    }

    if (send(this->sock, bytes, byteslength, 0) < 0)
    {
        if (this->onError)
            this->onError("Error: Cannot send the message.");

        perror("send");

        return;
    }
}

void
Socket::Connect(
    std::string ipv4,
    uint16_t port,
    std::function<void()> onConnected
) {
    if (inet_pton(AF_INET, ipv4.c_str(), &this->address.sin_addr) <= 0)
    {
        if (this->onError)
            this->onError("Invalid address. Address type not supported.");

        perror("inet_pton");

        return;
    }

    this->Connect((uint32_t)this->address.sin_addr.s_addr, port, onConnected);
}


void
Socket::Connect(
    uint32_t IPv4,
    uint16_t port,
    std::function<void()> onConnected
) {
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = IPv4;

    // Try to connect.
    if (connect(this->sock, (const sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        if (this->onError)
            this->onError("Connection failed to the host.");
        perror("connect");

        return;
    }

    // Connected to the server, fire the event.
    if (onConnected)
        onConnected();
    this->Listen();
}

void
Socket::Listen()
{
    // Start listening the socket from thread.
    std::thread receiveListening(Receive, this);
    receiveListening.detach();
}

void
Socket::setAddressStruct(
    sockaddr_in addr
) {
    this->address = addr;
}

void
Socket::Receive(
    Socket *socket
) {
    char tempBuffer[BUFFER_SIZE];
    int messageLength;
    while ((messageLength = recv(socket->sock, tempBuffer, BUFFER_SIZE, 0)) >= 0)
    {
        if (messageLength > 0)
        {
            if (socket->onMessageReceived)
                socket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength));
        }
        else
        {
            socket->Close();
            if (socket->onSocketClosed)
                socket->onSocketClosed();
        }
    }
    if (!socket->isClosed)
    {
        if (socket->onError)
            socket->onError(("Socket error! Error no (errno):" + errno));
        perror("recv");

        return;
    }
}
