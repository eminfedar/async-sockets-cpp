# Simple-Socket
Simple header-only Socket class in C++ with TCP & UDP.

# Usage
### TCP Client:
```c_cpp
#include "socket.h"

int main()
{
    // Initialize socket.
    Socket TCPSocket(Socket::TCP);

    // Connect to the host.
    TCPSocket.Connect("127.0.0.1", 8888);

    // Send String:
    TCPSocket.Send("Test");
    
    // Send Byte Array (char*):
    char byteArray[] = {65, 66, 67, 68, 69, '\0'};
    TCPSocket.Send(byteArray);
    
    // Receive package with length 1024.
    char* message = TCPSocket.Receive(1024);
    cout << "Message: " << message << endl;
    
    // You should delete message after usage:
    delete message;

    // Close the socket & connection.
    TCPSocket.Close();

    return 0;
}


```

### UDP Client:
```c_cpp
#include "socket.h"

int main()
{
    // Initialize socket.
    Socket UDPSocket(Socket::UDP);

    // Connect to the host. (not like a real TCP connection, just remembering the host)
    UDPSocket.Connect("127.0.0.1", 8888);

    // Send String:
    UDPSocket.Send("Test");

    // Send Byte Array (char*):
    char byteArray[] = {65, 66, 67, 68, 69, '\0'};
    UDPSocket.Send(byteArray);

    // Receive package with length 1024.
    char* message = UDPSocket.Receive(1024);
    cout << "Message: " << message << endl;

    // You should delete message after usage:
    delete message;

    // Close the socket.
    UDPSocket.Close();

    return 0;
}

```
