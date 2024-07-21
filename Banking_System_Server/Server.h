#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTextStream>
#include <ServerHandler.h>
#include <QThread>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    // Constructor
    explicit Server(QObject *parent = nullptr);

    // Starts the server and begins listening for incoming connections
    void startServer();

private:
    QTextStream qin;   // Stream for reading data from clients
    QTextStream qout;  // Stream for sending data to clients
    qint32 port;       // Port number on which the server will listen for connections

    // QTcpServer interface
protected:
    // Called when a new incoming connection is detected
    // handle: the socket descriptor for the incoming connection
    void incomingConnection(qintptr handle) override;
};

#endif // SERVER_H
