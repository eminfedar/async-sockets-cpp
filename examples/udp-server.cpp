#include "../easysocket/udpserver.h"
#include <iostream>

using namespace std;

int main()
{
    try
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
    }
    catch (const char *exception)
    {
        std::cerr << exception << std::endl;
    }

    return 0;
}
