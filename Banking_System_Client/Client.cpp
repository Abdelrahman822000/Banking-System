#include "Client.h"

// Constructor for the Client class
Client::Client(QObject *parent)
    : QObject{parent} // Initialize QObject with the parent pointer
{
    // Connect socket signals to the appropriate slots in this class
    connect(&socket, &QTcpSocket::connected, this, &Client::onConnection);
    connect(&socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
    connect(&socket, &QTcpSocket::errorOccurred, this, &Client::onErrorOccurred);
    connect(&socket, &QTcpSocket::stateChanged, this, &Client::onStateChanged);
    connect(&socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);

    // Initialize status flags and other member variables
    loginConfirmed = false;
    AdminCheck = false;
    UserCheck = false;
    UsernameCheck = false;
    PasswordCheck = false;
    newUserCheck = false;
    updateUserCheck = false;
    deletedUserCheck = false;
    accountNumberCheck = false;
    makeTransactionCheck = false;
    transferCheck = false;
    databaseCheck = false;

    accountNumber = QString();
    currentUser = QString();
    transferMessage = QString();
    transactionHistory = QString();
    database = QString();

    balance = 0; // Initialize balance to 0
}

// Connect to the specified server IP address and port
void Client::ConnectToDevice(QString IP, qint32 Port)
{
    if (socket.isOpen()) // Check if socket is already open
    {
        if ((ip == IP) && (port == Port)) // Check if the current IP and port match the new ones
        {
            qDebug() << "Client Connected already" << Qt::endl;
            return;
        }
        else
        {
            qDebug() << "Client was Connected to ip: " << ip << " and port: " << port << Qt::endl;
            socket.close(); // Close the existing connection

            port = Port; // Update the port and IP
            ip = IP;
            qDebug() << "Client now Connected to ip: " << IP << " and port: " << Port << Qt::endl;
            socket.connectToHost(ip, port); // Connect to the new IP and port
        }
    }
    else
    {
        this->port = Port; // Set the port and IP
        this->ip = IP;
        qDebug() << "Client Connected to ip: " << IP << " and port: " << port << Qt::endl;
        socket.connectToHost(this->ip, this->port); // Connect to the specified IP and port
    }
}

// Disconnect from the server
void Client::Disconnect()
{
    if (socket.isOpen()) // Check if socket is open
    {
        qDebug() << "Client Disconnected" << Qt::endl;
        socket.close(); // Close the connection
    }
}

// Send a request to the server with encrypted data
void Client::SendRequest(QString request, QString Data)
{
    qDebug() << "Before encryption:\nRequest: " << request << ", Data: " << Data;
    QString Request = encrypt(request); // Encrypt the request
    QString data = encrypt(Data); // Encrypt the data

    qDebug() << "After encryption:\nRequest: " << Request << ", Data: " << data;

    if (socket.isOpen()) // Check if socket is open
    {
        QJsonObject jsonObj;
        jsonObj["file_type"] = Request; // Add request type to JSON object
        jsonObj["file_size"] = data.toUtf8().size(); // Add data size to JSON object
        jsonObj["file_data"] = data; // Add encrypted data to JSON object

        QByteArray byteArray = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact); // Convert JSON object to QByteArray
        QString header = QString("JSonSize:%1/").arg(byteArray.size()); // Create header with size information

        byteArray.prepend(header.toUtf8()); // Prepend header to the byte array

        socket.write(byteArray); // Write byte array to socket
    }
}

// Get the local IP addresses of the client machine
QString Client::getLocalIPAddresses()
{
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses(); // Get all IP addresses

    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList[i] != QHostAddress::LocalHost && ipAddressesList[i].toIPv4Address()) // Skip localhost and select IPv4 address
        {
            qDebug() << "Local IP address: " << ipAddressesList[i].toString();
            return ipAddressesList[i].toString(); // Return the first valid IP address
        }
    }

    // If no valid IP address found, use IPv4 localhost
    qDebug() << "Local IP address: " << QHostAddress(QHostAddress::LocalHost).toString();
    return QHostAddress(QHostAddress::LocalHost).toString(); // Return localhost IP address
}

// Encrypt the input string (increment each character by 1)
QString Client::encrypt(const QString &input)
{
    QString result;

    for (int i = 0; i < input.size(); ++i)
    {
        QChar currentChar = input[i];
        if (currentChar != ',' && currentChar != '+') // Skip commas and +
        {
            result += QChar(currentChar.unicode() + 1); // Increment character by 1
        }
        else
        {
            result += currentChar; // Keep commas and + unchanged
        }
    }
    return result;
}

// Decrypt the input string (decrement each character by 1)
QString Client::decrypt(const QString &input)
{
    QString result;
    for (int i = 0; i < input.size(); ++i)
    {
        QChar currentChar = input[i];
        if (currentChar != ',' && currentChar != '+') // Skip commas and +
        {
            result += QChar(currentChar.unicode() - 1); // Decrement character by 1
        }
        else
        {
            result += currentChar; // Keep commas and + unchanged
        }
    }
    return result;
}

// Slot called when the connection to the server is established
void Client::onConnection()
{
    emit Connection(); // Emit the Connection signal
}

// Slot called when disconnected from the server
void Client::onDisconnected()
{
    emit Disconnected(); // Emit the Disconnected signal
}

// Slot called when an error occurs with the socket
void Client::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    emit ErrorOccurred(socketError); // Emit the ErrorOccurred signal with the error details
}

// Slot called when the socket state changes
void Client::onStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState) // If socket is in UnconnectedState
    {
        socket.close(); // Close the socket
    }
    emit StateChanged(socketState); // Emit the StateChanged signal with the new state
}

// Slot called when data is ready to be read from the socket
void Client::onReadyRead()
{
    QByteArray byteArray = socket.readAll(); // Read all data from the socket

    QString str = QString(byteArray); // Convert byte array to QString
    qint32 jsonSize = str.split("/")[0].split(":")[1].toInt(); // Extract JSON size from the header

    QString data = str.right(jsonSize); // Extract the JSON data

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8()); // Convert JSON data to QJsonDocument
    QJsonObject jsonObj = jsonDoc.object(); // Get the JSON object from the document

    qint32 dataSize = jsonObj["file_size"].toInt(); // Get the size of the data
    QString encryptedMessage = jsonObj["file_data"].toString(); // Get the encrypted message

    qDebug() << "Received data before Decryption=> " << encryptedMessage << Qt::endl;
    QString realMessage = decrypt(encryptedMessage); // Decrypt the message

    if (dataSize == realMessage.size()) // Check if the decrypted message size matches the expected size
    {
        qDebug() << "Received data after Decryption=> " << realMessage << Qt::endl;
        emit ReadyRead(realMessage); // Emit the ReadyRead signal with the decrypted message
    }
}
