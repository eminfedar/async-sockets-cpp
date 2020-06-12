#include <udpsocket.h>
#include <iostream>

using namespace std;

int main()
{
    // Our constants:
    const string IP = "localhost";
    const uint16_t PORT = 8888;

    int received = 0;

    // Initialize socket.
    UDPSocket udpSocket(true); // "true" to use Connection on UDP. Default is "false".
    udpSocket.Connect(IP, PORT);

    // Send String:
    udpSocket.Send("ABCDEFGH");

    udpSocket.onRawMessageReceived = [&](const char* message, int length, string ipv4, uint16_t port) {
        if (received++ == 100000)
            exit(0);
        else
            udpSocket.Send("ABCDEFGH");
        
        FDR_UNUSED(message);
        FDR_UNUSED(length);
        FDR_UNUSED(ipv4);
        FDR_UNUSED(port);
    };

    // You should do an input loop so the program will not terminated immediately:
    string input;
    getline(cin, input);
    while (input != "exit")
    {
        udpSocket.Send(input);
        getline(cin, input);
    }

    // Close the socket.
    udpSocket.Close();

    return 0;
}
