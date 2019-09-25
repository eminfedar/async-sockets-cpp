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
