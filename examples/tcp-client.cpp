#include "../easysocket/socket.h"
#include <iostream>

using namespace std;

int main()
{
    // Initialize socket.
    Socket tcpSocket;

    // Connect to the host.
    tcpSocket.Connect("127.0.0.1", 8888, [&] {
        cout << "Connected to the server successfully." << endl;

        // Send String:
        tcpSocket.Send("Hello Server!");
    });

    // Start receiving from the host.
    tcpSocket.onMessageReceived = [&](string message) {
        cout << "Message from the Server: " << message << endl;
    };

    // On socket closed:
    tcpSocket.onSocketClosed = [&]() {
        cout << "Connection lost with the server!" << endl;
    };

    // Check if there is any error:
    tcpSocket.onError = [&](string error) {
        cerr << error << endl;
    };

    // You should do an input loop so the program will not end immediately:
    // Because socket listenings are non-blocking.
    string input;
    cin >> input;
    while (input != "exit")
    {
        cout << input << endl;
        tcpSocket.Send(input);
        cin >> input;
    }

    tcpSocket.Close();

    return 0;
}
