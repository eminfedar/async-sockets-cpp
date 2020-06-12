#include <QCoreApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int received = 0;

    QUdpSocket* udpClient = new QUdpSocket();
    udpClient->connect(udpClient, &QUdpSocket::readyRead, [&]{
        while (udpClient->hasPendingDatagrams()) {
            QNetworkDatagram datagram = udpClient->receiveDatagram();
            received++;
            //qDebug() << received;
            if( received == 100000)
            {
                exit(0);
            } else {
                udpClient->write("ABCDEFGH", 8);
            }
        }
    });

    udpClient->connectToHost("localhost", 14530);

    udpClient->write("ABCDEFGH", 8);

    return a.exec();
}
