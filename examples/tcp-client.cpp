#include "socket.h"

int main()
{
    // Initialize socket.
    Socket TCPSocket(Socket::TCP);

    // Connect to the host.
    TCPSocket.Connect("127.0.0.1", 8888);

    // Send String:
    TCPSocket.Send("Test");

    // Send Byte Array (char*):
    char byteArray[] = {65, 66, 67, 68, 69, '\0'};
    TCPSocket.Send(byteArray);

    // Receive package with length 1024.
    char* message = TCPSocket.Receive(1024);

    // !!! You should delete message after usage: !!!
    delete message;

    // Close the socket & connection.
    TCPSocket.Close();

    return 0;
}
