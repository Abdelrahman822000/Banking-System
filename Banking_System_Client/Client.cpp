#include "Client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{
    connect(&socket,&QTcpSocket::connected,this,&Client::onConnection);
    connect(&socket,&QTcpSocket::disconnected,this,&Client::onDisconnected);
    connect(&socket,&QTcpSocket::errorOccurred,this,&Client::onErrorOccurred);
    connect(&socket,&QTcpSocket::stateChanged,this,&Client::onStateChanged);
    connect(&socket,&QTcpSocket::readyRead,this,&Client::onReadyRead);

}

void Client::ConnectToDevice(QString IP, qint32 Port)
{
    if(socket.isOpen())
    {
        if((ip == IP)&&(port == Port))
        {
            qDebug() << "Client Connected already" << Qt::endl;
            return;
        }
        else
        {
            qDebug() << "Client was Connected to ip: " << ip << " and port: " << port << Qt::endl;
            socket.close();

            port = Port;
            ip = IP;
            qDebug() << "Client now Connected to ip: " << IP << " and port: " << Port << Qt::endl;
            socket.connectToHost(ip,port);
        }
    }
    else
    {
        this->port = Port;
        this->ip = IP;
        qDebug() << "Client Connected to ip: " << IP << " and port: " << port << Qt::endl;
        socket.connectToHost(this->ip,this->port);
    }
}

void Client::Disconnect()
{
    if(socket.isOpen())
    {
        qDebug() << "Client Disconnected" << Qt::endl;
        socket.close();
    }
}

void Client::SendRequest(QString Request, QString data)
{
    if(socket.isOpen())
    {
        QJsonObject jsonObj;
        jsonObj["file_type"] = Request;
        jsonObj["file_size"] = data.toUtf8().size();
        jsonObj["file_data"] = data;

        QByteArray byteArray = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);
        QString header = QString("JSonSize:%1/").arg(byteArray.size());

        byteArray.prepend(header.toUtf8());

        socket.write(byteArray);
    }
}

QString Client::getLocalIPAddresses()
{
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList[i] != QHostAddress::LocalHost && ipAddressesList[i].toIPv4Address())
        {
            qDebug() << "Local IP address: " << ipAddressesList[i].toString();
            return ipAddressesList[i].toString();
        }
    }

    // If we did not find one, use IPv4 localhost
    qDebug() << "Local IP address: " << QHostAddress(QHostAddress::LocalHost).toString();
    return QHostAddress(QHostAddress::LocalHost).toString();
}

void Client::onConnection()
{
    emit Connection();
}

void Client::onDisconnected()
{
    emit Disconnected();
}

void Client::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    emit ErrorOccurred(socketError);
}

void Client::onStateChanged(QAbstractSocket::SocketState socketState)
{
    if(socketState == QAbstractSocket::UnconnectedState)
    {
        socket.close();
    }
    emit StateChanged(socketState);
}

void Client::onReadyRead()
{
    QByteArray byteArray = socket.readAll();

    QString str = QString(byteArray);
    qint32 jsonSize = str.split("/")[0].split(":")[1].toInt();

    QString data = str.right(jsonSize);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    qint32 dataSize = jsonObj["file_size"].toInt();
    QString realMessage = jsonObj["file_data"].toString();

    if(dataSize == realMessage.size())
    {
        qDebug() << "Recieved data => " << realMessage << Qt::endl;
        emit ReadyRead(realMessage);
    }
}
