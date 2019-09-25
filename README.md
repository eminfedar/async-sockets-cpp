# Asynchronous Sockets for C++
Simple header-only, thread-based, super-lightweight, asynchronous Socket classes in C++ for TCP & UDP.
```cpp
// Initialize socket.
Socket tcpSocket;
```
Super Easy!

---

## Examples:

#### TCP Client:
[examples/tcp-client.cpp](https://github.com/eminfedar/async-sockets-cpp/blob/master/examples/tcp-client.cpp):
```cpp
#include "../easysocket/socket.h"
#include <iostream>

using namespace std;

int main()
{
    // Initialize socket.
    Socket tcpSocket;

    // Connect to the host.
    tcpSocket.Connect("127.0.0.1", 8888, [&] {
        cout << "Connected to the server successfully." << endl;

        // Send String:
        tcpSocket.Send("Hello Server!");
    });

    // Start receiving from the host.
    tcpSocket.onMessageReceived = [&](string message) {
        cout << "Message from the Server: " << message << endl;
    };

    // On socket closed:
    tcpSocket.onSocketClosed = [&]() {
        cout << "Connection lost with the server!" << endl;
    };

    // Check if there is any error:
    tcpSocket.onError = [&](string error) {
        cerr << error << endl;
    };

    // You should do an input loop so the program will not end immediately:
    // Because socket listenings are non-blocking.
    string input;
    cin >> input;
    while (input != "exit")
    {
        tcpSocket.Send(input);
        cin >> input;
    }

    tcpSocket.Close();

    return 0;
}

```

### UDP Client:
[examples/udp-client.cpp](https://github.com/eminfedar/async-sockets-cpp/blob/master/examples/udp-client.cpp):
```cpp
#include "../easysocket/udpsocket.h"
#include <iostream>

using namespace std;

int main()
{
    try
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

        udpSocket.onError = [&](string error) {
            cout << error << endl;
        };

        // You should do an input loop so the program will not terminated immediately:
        string input;
        cin >> input;
        while (input != "exit")
        {
            udpSocket.SendTo(input, IP, PORT);
            cin >> input;
        }

        // Close the socket.
        udpSocket.Close();
    }
    catch (const char *exception)
    {
        std::cerr << exception << std::endl;
    }

    return 0;
}

```
### TCP Server:
```cpp
#include "../easysocket/tcpserver.h"
#include "../easysocket/socket.h"
#include <iostream>

using namespace std;

int main()
{
    // Initialize server socket..
    TCPServer tcpServer;

    // When a new client connected:
    tcpServer.onNewConnection = [&](Socket *newClient) {
        cout << "New client: [";
        cout << newClient->remoteAddress() << ":" << newClient->remotePort() << "]" << endl;

        newClient->onMessageReceived = [newClient](string message) {
            cout << newClient->remoteAddress() << ":" << newClient->remotePort() << " => " << message << endl;
        };

        newClient->onError = [newClient](string error) {
            cout << "ERR on socket:" << newClient->remoteAddress() << " => " << error << endl;
        };

        newClient->onSocketClosed = [newClient]() {
            cout << "Socket closed:" << newClient->remoteAddress() << endl;
        };
    };

    tcpServer.onError = [&](string error) {
        cerr << error << endl;
    };

    // Bind the server to a port.
    tcpServer.Bind(8888);

    // Start Listening the server.
    tcpServer.Listen();

    // You should do an input loop so the program will not terminated immediately:
    string input;
    cin >> input;
    while (input != "exit")
    {
        cin >> input;
    }

    // Close the server before exiting the program.
    tcpServer.Close();

    return 0;
}

```

### UDP Server:
```cpp
#include "../easysocket/udpserver.h"
#include <iostream>

using namespace std;

int main()
{
        // Initialize server socket..
        UDPServer udpServer;

        udpServer.onMessageReceived = [&](string message, string ipv4, uint16_t port) {
            cout << ipv4 << ":" << port << " => " << message << endl;

            udpServer.SendTo("I got your UDP message!", ipv4, port);
        };

        udpServer.onError = [&](string error) {
            cerr << error << endl;
        };

        // Bind the server to a port.
        udpServer.Bind(8888);

        // You should do an input loop so the program will not terminated immediately:
        string input;
        cin >> input;
        while (input != "exit")
        {
            cin >> input;
        }

        udpServer.Close();

    return 0;
}

```

**NOTE: Any usage or improvement ideas, pull requests are welcome!** *(feel free to open issues if you want a feature or report a bug :))*
