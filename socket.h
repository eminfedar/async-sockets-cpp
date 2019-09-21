#ifdef __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#elif _WIN32
#include <winsock32.h>
#endif

#include <cstring>

class Socket
{
public:
    enum SocketType {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM
    };

    Socket(SocketType sockType = TCP)
    {
        if ((this->sock = socket(AF_INET, sockType, 0)) < 0)
            throw "Socket initialization Error.";
    }

    void Send(const char *bytes)
    {
        send(this->sock, bytes, strlen(bytes), 0);
    }

    void Connect(const char *IPv4String, const int port)
    {
        if (inet_pton(AF_INET, IPv4String, &this->serverAddress.sin_addr) <= 0)
            throw "Invalid address. Address type not supported.";

        this->Connect((uint32_t)this->serverAddress.sin_addr.s_addr, port);
    }
    void Connect(const uint32_t IPv4, const int port)
    {
        this->serverAddress.sin_family = AF_INET;
        this->serverAddress.sin_port = htons(8888);
        this->serverAddress.sin_addr.s_addr = IPv4;

        if (connect(this->sock, (struct sockaddr *)&this->serverAddress, sizeof(this->serverAddress)) < 0)
            throw "Connection failed to the host.";
    }

    char* Receive(uint32_t bufferSize)
    {
        char tempBuffer[bufferSize];
        
        int messageLength = read(this->sock, tempBuffer, bufferSize);

        char* received = new char[messageLength];
        strncpy( received, tempBuffer, messageLength );
        received[messageLength] = '\0';

        return received;
    }

    void Close()
    {
        close(this->sock);
    }

private:
    int sock = 0;
    struct sockaddr_in serverAddress;
};