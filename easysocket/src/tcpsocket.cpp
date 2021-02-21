#include <tcpsocket.h>
#include <string.h>
TCPSocket::TCPSocket(std::function<void(int, std::string)> onError, int socketId) : BaseSocket(onError, TCP, socketId)
{
    this->setTimeout(this->timeout);
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

void TCPSocket::Connect(std::string host, uint16_t port, std::function<void()> onConnected, std::function<void(int, std::string)> onError)
{
    struct addrinfo hints, *res, *it;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo(host.c_str(), NULL, &hints, &res)) != 0) {
        onError(errno, "Invalid address." + std::string(gai_strerror(status)));
        return;
    }

    for(it = res; it != NULL; it = it->ai_next)
    {
        if (it->ai_family == AF_INET) { // IPv4
            memcpy((void*)(&this->address), (void*)it->ai_addr, sizeof(sockaddr_in));
            break; // for now, just get first ip (ipv4).
        }
    }

    freeaddrinfo(res);

    this->Connect((uint32_t)this->address.sin_addr.s_addr, port, onConnected, onError);
}

void TCPSocket::Connect(uint32_t ipv4, uint16_t port, std::function<void()> onConnected, std::function<void(int, std::string)> onError)
{
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);
    this->address.sin_addr.s_addr = ipv4;

    // Try to connect.
    if (connect(this->sock, (const sockaddr *)&this->address, sizeof(sockaddr_in)) < 0)
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
sockaddr_in TCPSocket::getAddressStruct() const
{
    return this->address;
}

void TCPSocket::setTimeout(int seconds) {
    this->timeout = seconds;
    struct timeval tv;      
    tv.tv_sec = seconds;
    tv.tv_usec = 0;

    setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
    setsockopt(this->sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));
}
int TCPSocket::getTimeout() const{
    return this->timeout;
}

void TCPSocket::Receive(TCPSocket *socket)
{
    char tempBuffer[socket->BUFFER_SIZE];
    int messageLength;

    while ((messageLength = recv(socket->sock, tempBuffer, socket->BUFFER_SIZE, 0)) > 0)
    {
        tempBuffer[messageLength] = '\0';
        if(socket->onMessageReceived)
            socket->onMessageReceived(std::string(tempBuffer).substr(0, messageLength));
        
        if(socket->onRawMessageReceived)
            socket->onRawMessageReceived(tempBuffer, messageLength);
    }

    socket->Close();
    if(socket->onSocketClosed)
        socket->onSocketClosed();
}
