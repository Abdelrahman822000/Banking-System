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
#include <QRandomGenerator>
#include <QDateTime>
#include <QVector>
#include <QTextStream>
#include <QSslSocket>
#include <QProcess>

class ServerHandler : public QThread
{
    Q_OBJECT

public:
    // Constructor: initializes the ServerHandler with a unique ID and optional parent object
    explicit ServerHandler(int Id, QObject *parent = nullptr);

    // Sends a message to the client
    void sendMessageToClient(QString Message);

    // Handles various operations based on the provided operation string
    void Operations(QString operation);

    // Reads a JSON file and returns the JSON object
    QJsonObject readJsonFile(const QString &filePath);

    // Writes a JSON object to a file
    void writeJsonFile(const QString &filePath, const QJsonObject &jsonObject);

    // Validates user credentials against a JSON object
    QString validateCredentials(const QJsonObject &jsonObject, const QString &username, const QString &password);

    // Adds a new user to the system
    bool addUser(const QString &fullname, const QString &username, const QString &password, const QString &email, int age, const QString &Signature);

    // Generates a new unique account number
    qint64 generateAccountNumber();

    // Checks if an account number is unique
    bool isAccountNumberUnique(qint64 accountNumber, const QJsonObject &users);

    // Updates the details of an existing user
    bool updateUser(const QString &accountNumber, const QString &fullname, const QString &username, const QString &password, const QString &email,
                    int age, const QString &Signature);

    // Deletes a user from the system based on the account number
    bool deleteUser(const QString &accountNumber);

    // Retrieves the account number for a given username from an admin perspective
    QString getAccountNumberAdmin(const QString &username);

    // Retrieves the account number for a given username from a user perspective
    QString getAccountNumberUser(const QString &username);

    // Performs a transaction for a user
    bool makeTransaction(const QString &user, const QString &amount, const QString &operation);

    // Retrieves the balance for a given username
    qint32 getBalance(const QString &username);

    // Transfers an amount between two accounts and returns the result
    QString transferAmount(const QString &senderAccountNumber, const QString &receiverAccountNumber, const QString &amount,
                           const QString &senderSignature);

    // Retrieves and returns the transaction history for a given account number
    QString viewTransactionHistory(const QString &accountNumber, int count);

    // Retrieves and returns the entire bank database
    QString viewBankDatabase();

    // Encrypt the input string (for secure communication)
    QString encrypt(const QString &input);

    // Decrypt the input string (for secure communication)
    QString decrypt(const QString &input);

    // Sends an email with the specified details
    void sendEmail(const QString &sender, const QString &recipient, const QString &subject, const QString &body);

    // Retrieves the email associated with a given account number
    QString getEmail(const QString &accountNumber);

    // Check if given account number exists in the database
    bool doesAccountNumberExist(const QString &accountNumber);


public slots:
    // Slot that handles data read from the client socket
    void onReadyRead_slot();

    // Slot that handles client disconnection
    void onDisconnect_slot();

private:
    int ID;                // Unique identifier for the handler
    QTcpSocket *socket_ptr; // Pointer to the TCP socket used for communication

    // QThread interface
protected:
    // Overrides the run method to execute the thread's main loop
    void run() override;
};

#endif // SERVERHANDLER_H
