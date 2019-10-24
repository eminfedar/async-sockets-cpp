# Asynchronous Sockets for C++
Simple thread-based, asynchronous Socket classes in C++ for TCP & UDP.
```cpp
// Initialize the UDP socket.
UDPSocket udpSocket;

// Send String:
udpSocket.SendTo("Test", IP, PORT);
```
Super Easy!


# Examples:
You can just open the `examples/` folder in terminal and run `make` to get working programs of these examples:

## Clients:
---

### [TCP Client](https://github.com/eminfedar/async-sockets-cpp/blob/master/examples/tcp-client.cpp):
```cpp
#include <tcpsocket.h>
#include <iostream>

using namespace std;

int main()
{
    // Initialize socket.
    TCPSocket tcpSocket;

    // Start receiving from the host.
    tcpSocket.onMessageReceived = [](string message) {
        cout << "Message from the Server: " << message << endl;
    };

    // On socket closed:
    tcpSocket.onSocketClosed = []{
        cout << "Connection closed." << endl;
    };

    // Connect to the host.
    tcpSocket.Connect("127.0.0.1", 8888, [&] {
        cout << "Connected to the server successfully." << endl;

        // Send String:
        tcpSocket.Send("Hello Server!");
    });

    // You should do an input loop so the program will not end immediately:
    // Because socket listenings are non-blocking.
    string input;
    getline(cin, input);
    while (input != "exit")
    {
        tcpSocket.Send(input);
        getline(cin, input);
    }

    tcpSocket.Close();

    return 0;
}

```

### [UDP Client](https://github.com/eminfedar/async-sockets-cpp/blob/master/examples/udp-client.cpp):
```cpp
#include <udpsocket.h>
#include <iostream>

using namespace std;

int main()
{
    // Our constants:
    const string IP = "127.0.0.1";
    const uint16_t PORT = 8888;

    // Initialize socket.
    UDPSocket udpSocket;

    // Send String:
    udpSocket.SendTo("Test", IP, PORT);

    // Send Byte Array (char*):
    const char byteArray[] = {65, 66, 67, 68, 69, 120, 52, 55};
    udpSocket.SendTo(byteArray, 8, IP, PORT);

    udpSocket.onMessageReceived = [&](string message, string ipv4, uint16_t port) {
        cout << ipv4 << ":" << port << " => " << message << endl;
    };

    // You should do an input loop so the program will not terminated immediately:
    string input;
    getline(cin, input);
    while (input != "exit")
    {
        udpSocket.SendTo(input, IP, PORT);
        getline(cin, input);
    }

    // Close the socket.
    udpSocket.Close();

    return 0;
}

```

## Servers:
---
### [TCP Server](https://github.com/eminfedar/async-sockets-cpp/blob/master/examples/tcp-server.cpp):
```cpp
#include <tcpserver.h>
#include <iostream>

using namespace std;

int main()
{
    // Initialize server socket..
    TCPServer tcpServer;

    // When a new client connected:
    tcpServer.onNewConnection = [&](TCPSocket *newClient) {
        cout << "New client: [";
        cout << newClient->remoteAddress() << ":" << newClient->remotePort() << "]" << endl;

        newClient->onMessageReceived = [newClient](string message) {
            cout << newClient->remoteAddress() << ":" << newClient->remotePort() << " => " << message << endl;
        };

        newClient->onSocketClosed = [newClient]() {
            cout << "Socket closed:" << newClient->remoteAddress() << ":" << newClient->remotePort() << endl;
        };
    };

    // Bind the server to a port.
    tcpServer.Bind(8888);

    // Start Listening the server.
    tcpServer.Listen();

    // You should do an input loop so the program will not terminated immediately:
    string input;
    getline(cin, input);
    while (input != "exit")
    {
        getline(cin, input);
    }

    // Close the server before exiting the program.
    tcpServer.Close();

    return 0;
}

```

### [UDP Server](https://github.com/eminfedar/async-sockets-cpp/blob/master/examples/udp-server.cpp):
```cpp
#include <udpserver.h>
#include <iostream>

using namespace std;

int main()
{
    // Initialize server socket..
    UDPServer udpServer;

    // onMessageReceived will run when a message received with information of ip & port of sender:
    udpServer.onMessageReceived = [&](string message, string ipv4, uint16_t port) {
        cout << ipv4 << ":" << port << " => " << message << endl;

        // Just send without control:
        udpServer.SendTo("I got your message!", ipv4, port);
    };

    // Bind the server to a port.
    udpServer.Bind(8888);

    // You should do an input loop so the program will not terminated immediately:
    string input;
    getline(cin, input);
    while (input != "exit")
    {
        getline(cin, input);
    }

    udpServer.Close();

    return 0;
}

```
