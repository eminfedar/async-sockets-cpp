#include <udpserver.h>
#include <iostream>

using namespace std;

int main()
{
    // Initialize server socket..
    UDPServer udpServer;
    
    udpServer.onRawMessageReceived = [&](const char* message, int length, string ipv4, uint16_t port) {
        // Just send without control:
        udpServer.SendTo(message, length, ipv4, port);
    };
    

    // Bind the server to a port.
    udpServer.Bind(8888, [](int errorCode, string errorMessage) {
        cout << errorCode << " : " << errorMessage << endl;
    });

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
