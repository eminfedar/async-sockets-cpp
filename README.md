# Easy-Socket
Simple header-only Socket class in C++ for TCP & UDP.
```cpp
// Initialize socket.
Socket TCPSocket(Socket::TCP);
```
Super Easy!

---

### TCP Client:
```cpp
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
    
    // Receive packages with max length 1024
    char* message = TCPSocket.Receive(1024);
    
    // You should delete message after usage:
    delete message;

    // Close the socket & connection.
    TCPSocket.Close();

    return 0;
}


```

### UDP Client:
```cpp
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

    // Receive packages with max length 1024
    char* message = UDPSocket.Receive(1024);

    // You should delete message after usage:
    delete message;

    // Close the socket.
    UDPSocket.Close();

    return 0;
}

```
**NOTE: Any usage or improvement ideas are welcome!** *(you can open issues if you want a feature)*
