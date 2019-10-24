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
