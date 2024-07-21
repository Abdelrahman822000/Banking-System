#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkInterface>

// The Client class handles the connection to a server and manages communication with it
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    // Connect to the server with the specified IP address and port
    void ConnectToDevice(QString ip, qint32 port);

    // Disconnect from the server
    void Disconnect();

    // Send a request to the server with the specified request type and data
    void SendRequest(QString request, QString Data);

    // Get the local IP addresses of the client machine
    QString getLocalIPAddresses();

    // Encrypt the input string (for secure communication)
    QString encrypt(const QString &input);

    // Decrypt the input string (for secure communication)
    QString decrypt(const QString &input);

    // Flags for various checks and statuses
    bool loginConfirmed;
    bool AdminCheck;
    bool UserCheck;
    bool UsernameCheck;
    bool PasswordCheck;
    bool newUserCheck;
    bool updateUserCheck;
    bool deletedUserCheck;
    bool accountNumberCheck;
    bool makeTransactionCheck;
    bool transferCheck;
    bool databaseCheck;

    QString accountNumber;
    QString currentUser;
    QString transferMessage;
    QString transactionHistory;
    QString database;

    qint32 balance;

signals:
    // Signal emitted when the connection is successfully established
    void Connection();

    // Signal emitted when disconnected from the server
    void Disconnected();

    // Signal emitted when an error occurs
    void ErrorOccurred(QAbstractSocket::SocketError socketError);

    // Signal emitted when the socket state changes
    void StateChanged(QAbstractSocket::SocketState socketState);

    // Signal emitted when data is ready to be read from the socket
    void ReadyRead(QString data);

private slots:
    // Slot called when the connection is established
    void onConnection();

    // Slot called when disconnected from the server
    void onDisconnected();

    // Slot called when an error occurs
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

    // Slot called when the socket state changes
    void onStateChanged(QAbstractSocket::SocketState socketState);

    // Slot called when data is ready to be read from the socket
    void onReadyRead();

private:
    QString ip; // IP address of the server
    qint32 port; // Port number of the server
    QTcpSocket socket; // TCP socket for communication
};

#endif // CLIENT_H
