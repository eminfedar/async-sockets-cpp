#include "tcpserver.hpp"
#include <iostream>

using namespace std;

int main()
{
    // Initialize server socket..
    TCPServer<> tcpServer;

    // When a new client connected:
    tcpServer.onNewConnection = [&](TCPSocket<> *newClient) {
        cout << "New client: [";
        cout << newClient->remoteAddress() << ":" << newClient->remotePort() << "]" << endl;

        newClient->onMessageReceived = [newClient](string message) {
            cout << newClient->remoteAddress() << ":" << newClient->remotePort() << " => " << message << endl;
            newClient->Send("OK!");
        };
        
        // If you want to use raw bytes
        /*
        newClient->onRawMessageReceived = [newClient](const char* message, int length) {
            cout << newClient->remoteAddress() << ":" << newClient->remotePort() << " => " << message << "(" << length << ")" << endl;
            newClient->Send("OK!");
        };
        */
        
        newClient->onSocketClosed = [newClient](int errorCode) {
            cout << "Socket closed:" << newClient->remoteAddress() << ":" << newClient->remotePort() << " -> " << errorCode << endl;
            cout << flush;
        };
    };

    // Bind the server to a port.
    tcpServer.Bind(8888, [](int errorCode, string errorMessage) {
        // BINDING FAILED:
        cout << errorCode << " : " << errorMessage << endl;
    });

    // Start Listening the server.
    tcpServer.Listen([](int errorCode, string errorMessage) {
        // LISTENING FAILED:
        cout << errorCode << " : " << errorMessage << endl;
    });

    // You should do an input loop, so the program won't terminate immediately
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
