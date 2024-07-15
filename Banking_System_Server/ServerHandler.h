#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTcpSocket>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class ServerHandler : public QThread
{
    Q_OBJECT
public:
    explicit ServerHandler(int Id,QObject *parent = nullptr);
    void sendMessageToClient(QString message);
    void Operations(QString operation);
    QJsonObject readJsonFile(const QString &filePath);
    QString validateCredentials(const QJsonObject &jsonObject, const QString &username, const QString &password);

public slots:
    void onReadWrite_slot();
    void onDisconnect_slot();

private:
    int ID;
    QTcpSocket *socket_ptr;
    // QThread interface
protected:
    void run();
};

#endif // SERVERHANDLER_H
