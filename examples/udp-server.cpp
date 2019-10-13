#include <udpserver.h>
#include <iostream>

using namespace std;

int main()
{
        // Initialize server socket..
        UDPServer udpServer;

        udpServer.onMessageReceived = [&](string message, string ipv4, uint16_t port) {
            cout << ipv4 << ":" << port << " => " << message << endl;

            udpServer.SendTo("I got your UDP message!", ipv4, port,
                // Error handling while sending:
                [](int errorCode, std::string errorMessage){
                    cerr << errorMessage << endl;
                }
            );
        };

        // Bind the server to a port.
        udpServer.Bind(8888, [](int errorCode, std::string errorMessage){
            cerr << errorMessage << endl;
        });

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
