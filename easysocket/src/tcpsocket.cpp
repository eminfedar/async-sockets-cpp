#include <tcpsocket.h>
TCPSocket::TCPSocket(std::function<void(int, std::string)> onError, int socketId) : BaseSocket(onError, TCP, socketId)
{
}

int TCPSocket::Send(std::string message)
{
    return this->Send(message.c_str(), message.length());
}

int TCPSocket::Send(const char *bytes, size_t byteslength)
{
    if (this->isClosed)
        return -1;

    int sent = 0;
    if ((sent = send(this->sock, bytes, byteslength, 0)) < 0)
    {
        perror("send");
    }
    return sent;
}

void TCPSocket::Connect(std::string ipv4, uint16_t port, std::function<void()> onConnected, std::function<void(int, std::string)> onError)
{
    if (inet_pton(AF_INET, ipv4.c_str(), &this->address.sin_addr) <= 0)
    {
        onError(errno, "Invalid address. Address type not supported.");

        return;
    }

    this->Connect((uint32_t)this->address.sin_addr.s_addr, port, onConnected, onError);
}

void TCPSocket::Connect(uint32_t IPv4, uint16_t port, std::function<void()> onConnected, std::function<void(int, std::string)> onError)
{
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = IPv4;

    // Try to connect.
    if (connect(this->sock, (const sockaddr *)&this->address, sizeof(this->address)) < 0)
    {
        onError(errno, "Connection failed to the host.");
        return;
    }

    // Connected to the server, fire the event.
    onConnected();

    // Start listening from server:
    this->Listen();
}

void TCPSocket::Listen()
{
    // Start listening the socket from thread.
    std::thread receiveListening(Receive, this);
    receiveListening.detach();
}

void TCPSocket::setAddressStruct(sockaddr_in addr)
{
    this->address = addr;
}

void TCPSocket::Receive(TCPSocket *socket)
{
    char tempBuffer[BUFFER_SIZE];
    int messageLength;

    while ((messageLength = recv(socket->sock, tempBuffer, BUFFER_SIZE, 0)) > 0)
    {
        socket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength));
    }

    socket->Close();
    socket->onSocketClosed();
}
