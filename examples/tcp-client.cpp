#include "tcpsocket.hpp"
#include <iostream>

using namespace std;

int main()
{
    // Initialize socket.
    TCPSocket tcpSocket([](int errorCode, std::string errorMessage){
        cout << "Socket creation error:" << errorCode << " : " << errorMessage << endl;
    });

    // Start receiving from the host.
    tcpSocket.onRawMessageReceived = [](const char* message, int length) {
        cout << "Message from the Server: " << message << "(" << length << ")" << endl;
    };

    // If you want to use std::string instead of const char*:
    //tcpSocket.onMessageReceived = [](string message) {
    //    cout << "Message from the Server: " << message << endl;
    //};
    
    // On socket closed:
    tcpSocket.onSocketClosed = [](int errorCode){
        cout << "Connection closed: " << errorCode << endl;
    };

    // Connect to the host (with a custom buffer size).
    tcpSocket.Connect<0xFFFF>("localhost", 8888, [&] {
        cout << "Connected to the server successfully." << endl;

        // Send String:
        tcpSocket.Send("Hello Server!");
    },
    [](int errorCode, std::string errorMessage){
        // CONNECTION FAILED
        cout << errorCode << " : " << errorMessage << endl;
    });

    // You should do an input loop, so the program won't terminate immediately
    string input;
    getline(cin, input);
    while (input != "exit")
    {
        tcpSocket.Send(input);
        getline(cin, input);
    }

    tcpSocket.Close();

    return 0;
}
