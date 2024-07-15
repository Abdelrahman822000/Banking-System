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
    explicit Server(QObject *parent = nullptr);
    void startServer();
private:
    QTextStream qin,qout;
    qint32 port;

    // QTcpServer interface
protected:
    void incomingConnection(qintptr handle);
};

#endif // SERVER_H
