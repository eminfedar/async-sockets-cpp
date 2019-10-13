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
            cout << "Socket closed:" << newClient->remoteAddress() << endl;
        };
    };

    // Bind the server to a port.
    tcpServer.Bind(8888, [](int errorCode, std::string errorMessage){
        cerr << errorMessage << endl;
    });

    // Start Listening the server.
    tcpServer.Listen([](int errorCode, std::string errorMessage){
        cerr << errorMessage << endl;
    });

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
