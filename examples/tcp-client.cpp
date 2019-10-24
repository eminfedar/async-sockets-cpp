#include <tcpsocket.h>
#include <iostream>

using namespace std;

int main()
{
    // Initialize socket.
    TCPSocket tcpSocket;

    // Start receiving from the host.
    tcpSocket.onMessageReceived = [](string message) {
        cout << "Message from the Server: " << message << endl;
    };

    // On socket closed:
    tcpSocket.onSocketClosed = []{
        cout << "Connection closed." << endl;
    };

    // Connect to the host.
    tcpSocket.Connect("127.0.0.1", 8888, [&] {
        cout << "Connected to the server successfully." << endl;

        // Send String:
        tcpSocket.Send("Hello Server!");
    });

    // You should do an input loop so the program will not end immediately:
    // Because socket listenings are non-blocking.
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
