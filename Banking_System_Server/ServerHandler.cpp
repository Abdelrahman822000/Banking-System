#include "ServerHandler.h"

ServerHandler::ServerHandler(int Id,QObject *parent)
    : QThread{parent},ID{Id}
{
}


void ServerHandler::run()
{
    qDebug() << "Client " << ID << " :is running on thread :" << QThread::currentThreadId();

    socket_ptr = new QTcpSocket();
    socket_ptr->setSocketDescriptor(ID);

    connect(socket_ptr,&QTcpSocket::readyRead,this,&ServerHandler::onReadWrite_slot,Qt::DirectConnection);
    connect(socket_ptr,&QTcpSocket::disconnected,this,&ServerHandler::onDisconnect_slot,Qt::DirectConnection);

    sendMessageToClient("Hello Client Server is greeting...");
    exec();

}


void ServerHandler::sendMessageToClient(QString message)
{
    if(socket_ptr->isOpen())
    {
        QJsonObject jsonObj;
        jsonObj["file_type"] ="Message";
        jsonObj["file_size"] = message.toUtf8().size();
        jsonObj["file_data"] =message;

        QByteArray byteArr = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);
        QString header = QString("JSonSize:%1/").arg(byteArr.size());

        byteArr.prepend(header.toUtf8());

        socket_ptr->write(byteArr);
    }
}

void ServerHandler::Operations(QString operation)
{
    auto data = operation.split(",");
    QString Operation = operation.split(",")[0];
    if(Operation == "Login")
    {
        QString userName = operation.split(",")[1];
        QString Password = operation.split(",")[2];

        QJsonObject Login = readJsonFile("..\\..\\..\\Database\\Login.json");
        QString Login_state = validateCredentials(Login, userName, Password);
        sendMessageToClient(Login_state);

    }
    else if(Operation == "Create User")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "Update User")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "Delete User")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "View Transaction History")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "View Account Balance")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "View Bank Database")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "Get Account Number")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "Transfer Amount")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "Make Transaction")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "Get Account Number")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "View Account Balance")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else if(Operation == "View Transaction History")
    {
        sendMessageToClient("My name is Abdelrahman");
    }
    else
    {
        sendMessageToClient("Wrong request");
    }
}

QJsonObject ServerHandler::readJsonFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open the file.");
        return QJsonObject();
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    return doc.object();

}

QString ServerHandler::validateCredentials(const QJsonObject &jsonObject, const QString &username, const QString &password)
{
    // Check in Admins
    if (jsonObject.contains("Admins"))
    {
        QJsonObject admins = jsonObject.value("Admins").toObject();
        if (admins.contains(username))
        {
            QString storedPassword = admins.value(username).toObject().value("Password").toString();
            if (storedPassword == password)
            {
                qDebug() << "Request Approved: Login Done as Admin";
                return "Login Done as Admin";
            }
            else
            {
                qWarning() << "Request Denied: Login Failed, Incorrect password.";
                return "Login Failed: Incorrect password.";
            }
        }
    }

    // Check in Users (if needed)
    if (jsonObject.contains("Users"))
    {
        QJsonArray users = jsonObject.value("Users").toArray();
        for (const QJsonValue &user : users)
        {
            QJsonObject userObj = user.toObject();
            if (userObj.contains("Username") && userObj["Username"].toString() == username)
            {
                if (userObj.contains("Password") && userObj["Password"].toString() == password)
                {
                    qDebug() << "Request Approved: Login Done as User";
                    return "Login Done as User";
                }
                else
                {
                    qWarning() << "Request Denied: Login Failed, Incorrect password.";
                    return "Login Failed: Incorrect password.";
                }
            }
        }
    }

    qWarning() << "Request Denied:  Login Failed, Username not found.";
    return "Login Failed: Username not found.";
}

// JSonSize:JsonObjsize/{"file_data":message,"fie_size":Messagesize,"file_type":"Message"}
void ServerHandler::onReadWrite_slot()
{
    QByteArray byteArr = socket_ptr->readAll();
    QString str = QString(byteArr);

    auto splitStr = str.split("/");
    if (splitStr.size() < 1 || splitStr[0].split(":").size() < 2) {
        qDebug() << "Invalid data format received from client " << ID;
        return;
    }

    qint32 jsonSize = splitStr[0].split(":")[1].toInt();

    QString data = str.right(jsonSize);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data received from client " << ID;
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    qint32 dataSize = jsonObj["file_size"].toInt();
    QString realMessage = jsonObj["file_data"].toString();
    QString request = jsonObj["file_type"].toString();

    if (dataSize == realMessage.size())
    {
        qDebug() << "Received Request: " << request << "with Data :" << realMessage << "From client " << ID;
        Operations(request + "," + realMessage);
    }
    else {
        qDebug() << "Data size mismatch for client " << ID;
    }
}


void ServerHandler::onDisconnect_slot()
{
    if(socket_ptr->isOpen())
    {
        socket_ptr->disconnect();
        qDebug()<<"Client "<<ID<<" has been disconnected";
    }
}
