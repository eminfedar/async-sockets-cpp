#include "socket.h"

int main()
{
    // Initialize socket.
    Socket UDPSocket(Socket::UDP);

    // Connect to the host. (not like a real TCP connection.)
    UDPSocket.Connect("127.0.0.1", 8888);

    // Send String:
    UDPSocket.Send("Test");

    // Send Byte Array (char*):
    char byteArray[] = {65, 66, 67, 68, 69, '\0'};
    UDPSocket.Send(byteArray);

    // Receive package with length 1024.
    char* message = UDPSocket.Receive(1024);

    // !!! You should delete message after usage: !!!
    delete message;

    // Close the socket.
    UDPSocket.Close();

    return 0;
}
