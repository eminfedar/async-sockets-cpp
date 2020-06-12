#include <udpsocket.h>
#include <iostream>

using namespace std;

int main()
{
    // Our constants:
    const string IP = "localhost";
    const uint16_t PORT = 8888;

    // Initialize socket.
    UDPSocket udpSocket(true); // "true" to use Connection on UDP. Default is "false".
    udpSocket.Connect(IP, PORT);

    // Send String:
    udpSocket.Send("ABCDEFGH");
    //udpSocket.SendTo("ABCDEFGH", IP, PORT); // If you want to connectless

    udpSocket.onRawMessageReceived = [&](const char* message, int length, string ipv4, uint16_t port) {
        cout << ipv4 << ":" << port << " => " << message << "(" << length << ")" << endl;
    };
    
    // If you want to use std::string:
    /*
    udpSocket.onMessageReceived = [&](string message, string ipv4, uint16_t port) {
        cout << ipv4 << ":" << port << " => " << message << endl;
    };
    */

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
