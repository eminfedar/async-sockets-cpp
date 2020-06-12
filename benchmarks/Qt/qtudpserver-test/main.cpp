#include <QCoreApplication>
#include <QUdpSocket>
#include <QNetworkDatagram>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QUdpSocket* udpServer = new QUdpSocket();

    udpServer->connect(udpServer, &QUdpSocket::readyRead, [&]{
        while (udpServer->hasPendingDatagrams()) {
            QNetworkDatagram datagram = udpServer->receiveDatagram();
            datagram.setDestination(datagram.senderAddress(), datagram.senderPort());
            udpServer->writeDatagram(datagram.data(), datagram.senderAddress(), datagram.senderPort());
            //qDebug() << "PAKET GELDI";
        }
    });
    udpServer->bind(14530);

    qDebug() << "Server started 14530...";

    return a.exec();
}
