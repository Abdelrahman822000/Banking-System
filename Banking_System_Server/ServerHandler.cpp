#include "ServerHandler.h"

ServerHandler::ServerHandler(int Id,QObject *parent)
    : QThread{parent},ID{Id}
{}

void ServerHandler::run()
{
    // Open a log file for appending logs
    QFile logFile("../../../Log/server_log.txt");
    QTextStream logStream(&logFile);

    // Try to open the log file in append mode and check if it was successful
    if (logFile.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Log file opened successfully."; // Debug message indicating success
    }
    else
    {
        qWarning() << "Failed to open log file."; // Warning message if opening the file fails
    }

    // Prepare a log entry with the current timestamp and client connection info
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString clientRequest = "Client " + QString::number(ID) + " is connected to the server";
    QString requestLog = QString("[%1] : %2").arg(currentTime).arg(clientRequest);

    // Write the log entry to the log file
    logStream << requestLog << Qt::endl;
    logStream.flush();  // Ensure the log entry is written immediately
    logFile.close();    // Close the log file

    // Debug message indicating the client's thread ID
    qDebug() << "Client " << ID << " :is running on thread :" << QThread::currentThreadId();

    // Initialize a new QTcpSocket object for communication
    socket_ptr = new QTcpSocket();

    // Set the socket descriptor to the ID provided
    socket_ptr->setSocketDescriptor(ID);

    // Connect the readyRead signal to the onReadyRead_slot slot to handle incoming data
    connect(socket_ptr, &QTcpSocket::readyRead, this, &ServerHandler::onReadyRead_slot, Qt::DirectConnection);

    // Connect the disconnected signal to the onDisconnect_slot slot to handle disconnection
    connect(socket_ptr, &QTcpSocket::disconnected, this, &ServerHandler::onDisconnect_slot, Qt::DirectConnection);

    // Send a welcome message to the client
    sendMessageToClient("Hello to banking system...");

    // Enter the event loop to process incoming events
    exec();
}

void ServerHandler::sendMessageToClient(QString Message)
{
    // Debug output showing the original message before encryption
    qDebug() << "Before encryption:\nMessage: " << Message;

    // Encrypt the message
    QString message = encrypt(Message);

    // Debug output showing the encrypted message
    qDebug() << "After encryption:\nMessage: " << message;

    // Check if the socket is open before attempting to send data
    if (socket_ptr->isOpen())
    {
        // Create a JSON object to store message metadata and data
        QJsonObject jsonObj;
        jsonObj["file_type"] = "Message";      // Type of data being sent
        jsonObj["file_size"] = message.toUtf8().size();  // Size of the encrypted message
        jsonObj["file_data"] = message;        // Encrypted message data

        // Convert JSON object to a compact JSON byte array
        QByteArray byteArr = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);

        // Create a header that contains the size of the JSON byte array
        QString header = QString("JSonSize:%1/").arg(byteArr.size());

        // Prepend the header to the byte array
        byteArr.prepend(header.toUtf8());

        // Write the byte array (header + JSON data) to the socket
        socket_ptr->write(byteArr);
    }
}

void ServerHandler::Operations(QString operation)
{
    // Open log file to record operations
    QFile logFile("../../../Log/server_log.txt");
    QTextStream logStream(&logFile);
    if (logFile.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Log file opened successfully.";
    }
    else
    {
        qWarning() << "Failed to open log file.";
    }

    QString currentTime;
    QString requestLog;

    // Split the operation string to identify the specific operation and its parameters
    QString Operation = operation.split(",")[0];

    // Handle different types of operations
    if (Operation == "Login")
    {
        // Extract login credentials
        QString userName = operation.split(",")[1];
        QString Password = operation.split(",")[2];

        // Read user data and validate credentials
        QJsonObject Login = readJsonFile("../../../Database/Login.json");
        QString Login_state = validateCredentials(Login, userName, Password);

        // Log the login attempt result
        currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", " + Login_state);
        logStream << requestLog << Qt::endl;
        logStream.flush();

        // Send login result to client
        sendMessageToClient(Login_state);
    }
    else if (Operation == "Create User")
    {
        // Extract user details for creation
        QString userName = operation.split(",")[1];
        QString Password = operation.split(",")[2];
        QString FullName = operation.split(",")[3];
        QString Email = operation.split(",")[4];
        QString Age = operation.split(",")[5];
        QString Signature = operation.split(",")[6];

        // Attempt to add the new user
        bool userAdded = addUser(FullName, userName, Password, Email, Age.toInt(), Signature);

        if (userAdded)
        {
            // Log successful user creation
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: New user added successfully.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qDebug() << "Client " + QString::number(ID) + ", " << "Request Approved: New user added successfully.";

            // Send email confirmation
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "New user added successfully.");

            // Notify client of success
            sendMessageToClient("New user added successfully.");
        }
        else
        {
            // Log failed user creation attempt
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Denied: Username is not available.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qWarning() << "Request Denied: Username is not available.";
            sendMessageToClient("Can't create new user, Username is not available.");
        }
    }
    else if (Operation == "Update User")
    {
        // Extract user details for update
        QString userName = operation.split(",")[1];
        QString Password = operation.split(",")[2];
        QString FullName = operation.split(",")[3];
        QString Email = operation.split(",")[4];
        QString Age = operation.split(",")[5];
        QString AccountNumber = operation.split(",")[6];
        QString Signature = operation.split(",")[7];

        // Handle empty age field
        if (Age.isEmpty())
        {
            Age = '0';
        }

        // Attempt to update the user
        bool UserUpdated = updateUser(AccountNumber, FullName, userName, Password, Email, Age.toInt(), Signature);

        if (UserUpdated)
        {
            // Log successful user update
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: User updated successfully.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qDebug() << "Request Approved: User updated successfully.";

            // Send email confirmation
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "User updated successfully.");
            sendMessageToClient("User updated successfully.");
        }
        else
        {
            // Log failed user update attempt
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Denied: User not found.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qWarning() << "Request Denied: User not found.";
            sendMessageToClient("Can't update user, User not found.");
        }
    }
    else if (Operation == "Delete User")
    {
        // Extract account number for user deletion
        QString AccountNumber = operation.split(",")[1];

        // Get user email for notification
        QString Email = getEmail(AccountNumber);

        // Attempt to delete the user
        bool deletedUser = deleteUser(AccountNumber);

        if (deletedUser)
        {
            // Log successful user deletion
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: User deleted successfully.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qDebug() << "Request Approved: User deleted successfully.";

            // Send email confirmation
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "User deleted successfully.");
            sendMessageToClient("User deleted successfully.");
        }
        else
        {
            // Log failed user deletion attempt
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Denied: User not found.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qWarning() << "Request Denied: User not found.";
            sendMessageToClient("Can't delete user, User not found.");
        }
    }
    else if (Operation == "Get Account Number as admin")
    {
        // Extract username for account number retrieval
        QString userName = operation.split(",")[1];
        QString accountNumber = getAccountNumberAdmin(userName);

        if (!accountNumber.isEmpty())
        {
            // Log successful account number retrieval
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: User found sending account number.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qDebug() << "Request Approved: User found sending account number.";

            // Send email confirmation
            QString Email = getEmail(accountNumber);
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "User found sending account number\nAccount number: "  + accountNumber);

            // Notify client of the account number
            sendMessageToClient("Account number:"  + accountNumber);
        }
        else
        {
            // Log failed account number retrieval
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Denied: User not found.");
            logStream << requestLog << Qt::endl;
            logStream.flush();
            qWarning() << "Request Denied: User not found.";
            sendMessageToClient("Can't send account number, User not found.");
        }
    }
    else if (Operation == "Get Account Number as user")
    {
        // Extract username for account number retrieval
        QString userName = operation.split(",")[1];
        QString accountNumber = getAccountNumberUser(userName);

        if (!accountNumber.isEmpty())
        {
            // Log successful account number retrieval
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: User found sending account number.");
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qDebug() << "Request Approved: User found sending account number.";

            // Send email confirmation
            QString Email = getEmail(accountNumber);
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "User found sending account number\nAccount number: "  + accountNumber);

            // Notify client of the account number
            sendMessageToClient("Account number:"  + accountNumber);
        }
        else
        {
            // Log failed account number retrieval
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Denied: User not found.");
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qWarning() << "Request Denied: User not found.";
            sendMessageToClient("Can't send account number, User not found.");
        }
    }
    else if (Operation == "Make Transaction")
    {
        // Extract transaction details
        QString user = operation.split(",")[1];
        QString amount = operation.split(",")[2];
        QString transferOperation = operation.split(",")[3];

        // Retrieve account number and email for transaction
        QString accountNumber = getAccountNumberUser(user);
        QString Email = getEmail(accountNumber);

        // Attempt the transaction
        bool transactionDone = makeTransaction(user, amount, transferOperation);

        if (transactionDone)
        {
            // Log successful transaction
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: Transaction Done.");
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qDebug() << "Request Approved: Transaction Done.";

            // Send email confirmation based on the type of transaction
            if (transferOperation == "Deposit")
            {
                sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "Transaction Done\nDeposited: "  + amount + " to account: " + accountNumber);
            }
            else if (transferOperation == "Withdraw")
            {
                sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "Transaction Done\nWithdrawn: "  + amount + " from account: " + accountNumber);
            }

            // Notify client of transaction success
            sendMessageToClient("Transaction Done.");
        }
        else
        {
            // Log failed transaction attempt
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Denied: Transaction failed, Balance insufficient.");
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qWarning() << "Request Denied: Transaction failed, Balance insufficient.";

            // Send email notification of transaction failure
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "Transaction failed\nBalance insufficient.");
            sendMessageToClient("Transaction failed, Balance insufficient.");
        }
    }
    else if (Operation == "View Account Balance as user")
    {
        // Extract username and retrieve balance
        QString user = operation.split(",")[1];
        QString accountNumber = getAccountNumberUser(user);
        QString Email = getEmail(accountNumber);

        qint32 balance = getBalance(accountNumber);

        // Log balance retrieval
        QString strbalance = QString::number(balance);
        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: sending balance");
        logStream << requestLog << Qt::endl;
        logStream.flush();

        qDebug() << "Request Approved: sending balance";

        // Send email with balance details
        sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "Balance: " + strbalance);
        sendMessageToClient("Balance:" + strbalance);
    }
    else if (Operation == "View Account Balance as admin")
    {
        // Extract account number and retrieve balance
        QString accountNumber = operation.split(",")[1];
        QString Email = getEmail(accountNumber);

        bool accountNumberCheck = doesAccountNumberExist(accountNumber);
        qint32 balance = getBalance(accountNumber);
        if(accountNumberCheck)
        {
            // Log balance retrieval
            QString strbalance = QString::number(balance);
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: sending balance");
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qDebug() << "Request Approved: sending balance";

            // Send email with balance details
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", "Balance: " + strbalance);
            sendMessageToClient("Balance:" + strbalance);
        }
        else
        {
            qWarning() << "Request Denied: Account number not found";
            sendMessageToClient("Can\'t send account balance, Account number not found");
        }

    }
    else if (Operation == "Transfer Amount")
    {
        // Extract transaction details
        QString user = operation.split(",")[1];
        QString senderAccountNumber = getAccountNumberUser(user);
        QString receiverAccountNumber = operation.split(",")[2];
        QString amount = operation.split(",")[3];
        QString signature = operation.split(",")[4];

        // Retrieve emails for sender and receiver
        QString senderEmail = getEmail(senderAccountNumber);
        QString receiverEmail = getEmail(receiverAccountNumber);

        // Attempt to transfer amount
        QString transferMessage = transferAmount(senderAccountNumber, receiverAccountNumber, amount, signature);

        if (transferMessage.contains("Transfer Successful"))
        {
            // Log successful transfer
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Approved: Transfer Done.");
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qDebug() << "Request Approved: Transfer Done.";

            // Send email confirmations for successful transfer
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", senderEmail, "Server Response", "Transfer Successful\nTransferred amount: " + amount + " to account: " + receiverAccountNumber);
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", receiverEmail, "Server Response", "Transfer Successful\nReceived amount: " + amount + " from account: " + senderAccountNumber);
            sendMessageToClient("Transfer Successful.");
        }
        else if (transferMessage.contains("Transfer Failed"))
        {
            // Log failed transfer attempt
            QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Request Denied: " + transferMessage);
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qWarning() << "Request Denied: " + transferMessage;

            // Send email notification of transfer failure
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", senderEmail, "Server Response", transferMessage);
            sendMessageToClient(transferMessage);
        }
    }
    else if (Operation == "View Transaction History as user")
    {
        // Extract username and retrieve transaction history
        QString userName = operation.split(",")[1];
        QString accountNumber = getAccountNumberUser(userName);
        QString count = operation.split(",")[2];
        QString Email = getEmail(accountNumber);

        QString history = viewTransactionHistory(accountNumber, count.toInt());

        // Send email with transaction history
        sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", history);
        sendMessageToClient(history);
    }
    else if (Operation == "View Transaction History as admin")
    {
        // Extract account number and retrieve transaction history
        QString accountNumber = operation.split(",")[1];
        QString count = operation.split(",")[2];
        QString Email = getEmail(accountNumber);

        QString history = viewTransactionHistory(accountNumber, count.toInt());

        // Send email with transaction history
        if(!history.contains("Account number not found in database"))
        {
            sendEmail("abdelrahmanmohamedhamad17@gmail.com", Email, "Server Response", history);
        }

        sendMessageToClient(history);
    }
    else if (Operation == "View Bank Database")
    {
        // Retrieve and send the entire bank database
        QString database = viewBankDatabase();
        sendMessageToClient(database);
    }
    else
    {
        // Handle invalid operation request
        sendMessageToClient("Wrong request");
    }

    // Close the log file
    logFile.close();
}

QJsonObject ServerHandler::readJsonFile(const QString &filePath)
{
    // Create a QFile object with the specified file path
    QFile file(filePath);

    // Attempt to open the file in read-only mode
    if (!file.open(QIODevice::ReadOnly))
    {
        // If the file cannot be opened, log a warning and return an empty QJsonObject
        qWarning("Couldn't open the file.");
        return QJsonObject();
    }

    // Read all the data from the file into a QByteArray
    QByteArray jsonData = file.readAll();

    // Close the file after reading
    file.close();

    // Parse the JSON data into a QJsonDocument
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));

    // Return the JSON object from the parsed document
    return doc.object();
}

void ServerHandler::writeJsonFile(const QString &filePath, const QJsonObject &jsonObject)
{
    // Create a QFile object with the specified file path
    QFile file(filePath);

    // Attempt to open the file in write-only mode
    if (!file.open(QIODevice::WriteOnly))
    {
        // If the file cannot be opened, log a warning and exit the function
        qWarning("Couldn't open the file.");
        return;
    }

    // Create a QJsonDocument from the provided QJsonObject
    QJsonDocument doc(jsonObject);

    // Write the JSON data (from the QJsonDocument) to the file
    file.write(doc.toJson());

    // Close the file after writing
    file.close();
}

QString ServerHandler::validateCredentials(const QJsonObject &jsonObject, const QString &username, const QString &password)
{
    // Check if the JSON object contains the "Admins" key
    if (jsonObject.contains("Admins"))
    {
        // Retrieve the "Admins" object and check if it contains the given username
        QJsonObject admins = jsonObject.value("Admins").toObject();
        if (admins.contains(username))
        {
            // Retrieve the stored password for the given username
            QString storedPassword = admins.value(username).toObject().value("Password").toString();

            // Compare the stored password with the provided password
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

    // Check if the JSON object contains the "Users" key
    if (jsonObject.contains("Users"))
    {
        // Retrieve the "Users" object and check if it contains the given username
        QJsonObject users = jsonObject.value("Users").toObject();
        if (users.contains(username))
        {
            // Retrieve the stored password for the given username
            QJsonObject user = users.value(username).toObject();
            QString storedPassword = user.value("Password").toString();

            // Compare the stored password with the provided password
            if (storedPassword == password)
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

    // If the username is not found in either "Admins" or "Users"
    qWarning() << "Request Denied: Login Failed, Username not found.";
    return "Login Failed: Username not found.";
}

bool ServerHandler::addUser(const QString &fullname, const QString &username, const QString &password, const QString &email, int age, const QString &Signature)
{
    // Read the existing login data and user data from JSON files
    QJsonObject loginData = readJsonFile("../../../Database//Login.json");
    QJsonObject userData = readJsonFile("../../../Database//DataBase.json");

    // Retrieve Admins and Users from login data
    QJsonObject admins = loginData.value("Admins").toObject();
    QJsonObject users = loginData.value("Users").toObject();

    // Check if the username already exists in either Admins or Users
    bool usernameExists = admins.contains(username) || users.contains(username);

    // If the username exists, return false indicating failure to add the user
    if (usernameExists)
    {
        return false;
    }

    // Generate a unique account number
    qint64 accountNumber;
    do
    {
        accountNumber = generateAccountNumber(); // Function to generate a new account number
    } while (!isAccountNumberUnique(accountNumber, users)); // Ensure the account number is unique

    // Create a new user JSON object
    QJsonObject newUser;
    newUser["Username"] = username;
    newUser["Password"] = password;
    newUser["AccountNumber"] = QString::number(accountNumber);

    // Add the new user to the Users section in Login.json
    users[username] = newUser;
    loginData["Users"] = users;

    // Create a new user data JSON object for DataBase.json
    QJsonObject newUserData;
    QString accountNumberString = QString::number(accountNumber);

    newUserData["Fullname"] = fullname;
    newUserData["Username"] = username;
    newUserData["Email"] = email;
    newUserData["Age"] = age;
    newUserData["AccountNumber"] = accountNumberString;
    newUserData["Balance"] = 0; // Initialize balance to 0
    newUserData["Signature"] = Signature;

    // Add the new user data to the userData object
    userData[accountNumberString] = newUserData;

    // Write the updated JSON objects back to the files
    writeJsonFile("../../../Database//Login.json", loginData);
    writeJsonFile("../../../Database//DataBase.json", userData);

    // Return true indicating successful addition of the user
    return true;
}

qint64 ServerHandler::generateAccountNumber()
{
    // Generate a random 10-digit account number using QRandomGenerator
    // The range is between 1000000000 and 9999999999 (inclusive) to ensure a 10-digit number
    qint64 accountNumber = QRandomGenerator::global()->bounded(1000000000, 9999999999);

    return accountNumber; // Return the generated account number
}


bool ServerHandler::isAccountNumberUnique(qint64 accountNumber, const QJsonObject &users)
{
    QSet<qint64> accountNumbers; // Set to store account numbers for uniqueness check

    // Iterate over each user in the provided QJsonObject
    for (const QString &key : users.keys())
    {
        // Extract the account number from the user object and convert it to qint64
        qint64 userAccountNumber = users[key].toObject().value("AccountNumber").toString().toLongLong();
        // Insert the account number into the set
        accountNumbers.insert(userAccountNumber);
    }

    // Check if the given account number is not in the set of existing account numbers
    bool accountNumberUnique = !accountNumbers.contains(accountNumber);
    return accountNumberUnique; // Return the result of the uniqueness check
}

bool ServerHandler::updateUser(const QString &accountNumber, const QString &fullname, const QString &username, const QString &password, const QString &email, int age, const QString &Signature)
{
    // Read JSON files for user and login data
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");
    QJsonObject loginData = readJsonFile("../../../Database/Login.json");

    // Check if the provided account number exists in the user data
    if (!userData.contains(accountNumber))
    {
        return false; // Account number not found, return false
    }

    // Retrieve existing data for the user
    QJsonObject existingUserData = userData.value(accountNumber).toObject();
    QJsonObject existingUserLoginData = loginData.value("Users").toObject().value(existingUserData.value("Username").toString()).toObject();

    // Update user details if provided
    if (!fullname.isEmpty())
    {
        existingUserData["Fullname"] = fullname; // Update fullname if provided
    }
    if (!password.isEmpty())
    {
        existingUserLoginData["Password"] = password; // Update password if provided
    }
    if (!username.isEmpty())
    {
        // Remove the old username from Login.json
        QJsonObject users = loginData.value("Users").toObject();
        if (users.contains(existingUserData.value("Username").toString()))
        {
            users.remove(existingUserData.value("Username").toString());
        }

        // Update username in the login data and user data
        existingUserLoginData["Username"] = username;
        existingUserData["Username"] = username;

        // Add the new username to Login.json
        QJsonObject newUser;
        newUser["Username"] = username;
        newUser["Password"] = existingUserLoginData["Password"].toString();
        newUser["AccountNumber"] = accountNumber;
        users[username] = newUser;
        loginData["Users"] = users;
    }

    if (!email.isEmpty())
    {
        existingUserData["Email"] = email; // Update email if provided
    }
    if (age > 0)
    {
        existingUserData["Age"] = age; // Update age if provided and valid
    }
    if (!Signature.isEmpty())
    {
        existingUserData["Signature"] = Signature; // Update signature if provided
    }

    // Update the user data in DataBase.json
    userData[accountNumber] = existingUserData;

    // Write the updated data back to the JSON files
    writeJsonFile("../../../Database/DataBase.json", userData);
    writeJsonFile("../../../Database/Login.json", loginData);

    return true; // Successfully updated user, return true
}

bool ServerHandler::deleteUser(const QString &accountNumber)
{
    // Read JSON files for user and login data
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");
    QJsonObject loginData = readJsonFile("../../../Database/Login.json");

    // Check if the account number exists in DataBase.json
    if (!userData.contains(accountNumber))
    {
        return false; // Account number not found, return false
    }

    // Retrieve user data associated with the account number
    QJsonObject existingUserData = userData.value(accountNumber).toObject();
    QString username = existingUserData["Username"].toString(); // Extract the username

    // Remove the user data from DataBase.json
    userData.remove(accountNumber);

    // Remove the user from Login.json
    QJsonObject users = loginData.value("Users").toObject();
    if (users.contains(username))
    {
        users.remove(username); // Remove the user entry from Login.json
    }
    loginData["Users"] = users;

    // Write the updated data back to the JSON files
    writeJsonFile("../../../Database/DataBase.json", userData);
    writeJsonFile("../../../Database/Login.json", loginData);

    return true; // Successfully deleted user, return true
}

QString ServerHandler::getAccountNumberAdmin(const QString &username)
{
    // Read JSON file containing login data
    QJsonObject loginData = readJsonFile("../../../Database/Login.json");

    // Get the Users object from the login data
    QJsonObject users = loginData.value("Users").toObject();

    // Check if the username exists in the Users object
    if (users.contains(username))
    {
        // Retrieve the user object associated with the username
        QJsonObject user = users.value(username).toObject();

        // Return the account number of the user
        return user.value("AccountNumber").toString();
    }
    else
    {
        // Username not found, return an empty string
        return QString();
    }
}

QString ServerHandler::getAccountNumberUser(const QString &username)
{
    // Read JSON file containing login data
    QJsonObject loginData = readJsonFile("../../../Database/Login.json");

    // Get the Users object from the login data
    QJsonObject users = loginData.value("Users").toObject();

    // Retrieve the user object associated with the username
    QJsonObject user = users.value(username).toObject();

    // Return the account number of the user
    return user.value("AccountNumber").toString();
}

bool ServerHandler::makeTransaction(const QString &user, const QString &amount, const QString &operation)
{
    QFile logFile("../../../Log/server_log.txt");
    QTextStream logStream(&logFile);

    // Open the log file for appending, and handle errors if any
    if (logFile.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Log file opened successfully.";
    }
    else
    {
        qWarning() << "Failed to open log file.";
    }

    QString currentTime;
    QString requestLog;

    // Get the account number for the user
    QString accountNumber = getAccountNumberUser(user);

    // Read the DataBase.json file to get user data
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");

    // Retrieve the user's data from the JSON object
    QJsonObject userAccount = userData.value(accountNumber).toObject();

    // Get the current balance from the user's data
    qint32 balance = userAccount.value("Balance").toInt();

    // Convert the amount to integer for transaction
    qint32 transactionAmount = amount.toInt();

    // Perform the transaction based on the operation type
    if (operation == "Deposit")
    {
        // Increase balance for deposit operation
        balance += transactionAmount;
    }
    else if (operation == "Withdraw")
    {
        // Check if there is sufficient balance for withdrawal
        if (transactionAmount > balance)
        {
            // Log and report insufficient balance error
            currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Transaction Failed: Insufficient balance for withdrawal.");
            logStream << requestLog << Qt::endl;
            logStream.flush();

            qWarning() << "Transaction Failed: Insufficient balance for withdrawal. Current balance:" << balance << "Requested amount:" << transactionAmount;
            return false;
        }
        else
        {
            // Decrease balance for withdrawal operation
            balance -= transactionAmount;
        }
    }
    else
    {
        // If operation is not recognized, do nothing
    }

    // Update the balance in the user's data
    userAccount["Balance"] = balance;

    // Ensure the Transaction History field exists
    if (!userAccount.contains("Transaction History"))
    {
        userAccount["Transaction History"] = QJsonArray();
    }

    // Update the transaction history
    QJsonArray transactionHistory = userAccount.value("Transaction History").toArray();
    QJsonObject newTransaction;
    newTransaction["Date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    newTransaction["Details"] = (operation == "Deposit" ? "+" : "-") + QString::number(transactionAmount);
    transactionHistory.append(newTransaction);
    userAccount["Transaction History"] = transactionHistory;

    // Update the user's data in DataBase.json
    userData[accountNumber] = userAccount;

    // Write the updated user data back to DataBase.json
    writeJsonFile("../../../Database/DataBase.json", userData);

    // Log the successful transaction
    currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", Transaction Successful.");
    logStream << requestLog << Qt::endl;
    logStream.flush();

    qDebug() << "Transaction Successful: New balance for account" << accountNumber << "is" << balance;

    // Close the log file
    logFile.close();

    return true;
}

qint32 ServerHandler::getBalance(const QString &accountNumber)
{
    // Read the DataBase.json file to access user data
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");

    // Retrieve the user's data from the JSON object using the account number
    QJsonObject userAccount = userData.value(accountNumber).toObject();

    // Extract the current balance from the user's data
    qint32 balance = userAccount.value("Balance").toInt();

    // Return the current balance
    return balance;
}

QString ServerHandler::transferAmount(const QString &senderAccountNumber, const QString &receiverAccountNumber, const QString &amount, const QString &senderSignature)
{
    // Read the DataBase.json file to access user data
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");

    // Check if both sender and receiver account numbers exist in the database
    if (!userData.contains(senderAccountNumber) || !userData.contains(receiverAccountNumber))
    {
        qWarning() << "Transfer Failed: One or both account numbers not found in database.";
        return "Transfer Failed: One or both account numbers not found in database.";
    }

    // Check if the sender and receiver accounts are the same
    if (senderAccountNumber == receiverAccountNumber)
    {
        qWarning() << "Transfer Failed: Can't send from an account to itself.";
        return "Transfer Failed: Can't send from an account to itself.";
    }

    // Get the sender's data from the database
    QJsonObject senderAccount = userData.value(senderAccountNumber).toObject();
    QString storedSignature = senderAccount.value("Signature").toString(); // Retrieve the stored signature

    // Verify the sender's signature to ensure authenticity
    if (storedSignature != senderSignature)
    {
        qWarning() << "Transfer Failed: Invalid signature for sender: " << senderAccountNumber;
        return "Transfer Failed: Invalid signature for sender.";
    }

    // Convert the amount to qint32 and check if the conversion is successful
    bool ok;
    qint32 transferAmount = amount.toInt(&ok);
    if (!ok)
    {
        qWarning() << "Transfer Failed: Invalid amount" << amount;
        return "Transfer Failed: Invalid amount.";
    }

    // Retrieve the sender's current balance
    qint32 senderBalance = senderAccount.value("Balance").toInt();

    // Check if the sender has enough balance for the transfer
    if (transferAmount > senderBalance)
    {
        qWarning() << "Transfer Failed: Insufficient balance for transfer. Sender balance:" << senderBalance << "Requested amount:" << transferAmount;
        return "Transfer Failed: Insufficient balance for transfer.";
    }

    // Get the receiver's data from the database
    QJsonObject receiverAccount = userData.value(receiverAccountNumber).toObject();
    qint32 receiverBalance = receiverAccount.value("Balance").toInt();

    // Perform the transfer
    senderBalance -= transferAmount; // Deduct amount from sender's balance
    receiverBalance += transferAmount; // Add amount to receiver's balance

    // Update the balances in the user data
    senderAccount["Balance"] = senderBalance;
    receiverAccount["Balance"] = receiverBalance;

    // Ensure the Transaction History field exists for the sender
    if (!senderAccount.contains("Transaction History"))
    {
        senderAccount["Transaction History"] = QJsonArray();
    }

    // Update the transaction history for the sender
    QJsonArray senderTransactionHistory = senderAccount.value("Transaction History").toArray();
    QJsonObject senderTransaction;
    senderTransaction["Date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    senderTransaction["Details"] = QString::number(transferAmount) + " to " + receiverAccountNumber;
    senderTransactionHistory.append(senderTransaction);
    senderAccount["Transaction History"] = senderTransactionHistory;

    // Ensure the Transaction History field exists for the receiver
    if (!receiverAccount.contains("Transaction History"))
    {
        receiverAccount["Transaction History"] = QJsonArray();
    }

    // Update the transaction history for the receiver
    QJsonArray receiverTransactionHistory = receiverAccount.value("Transaction History").toArray();
    QJsonObject receiverTransaction;
    receiverTransaction["Date"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    receiverTransaction["Details"] = QString::number(transferAmount) + " from " + senderAccountNumber;
    receiverTransactionHistory.append(receiverTransaction);
    receiverAccount["Transaction History"] = receiverTransactionHistory;

    // Update the user data in the DataBase.json file
    userData[senderAccountNumber] = senderAccount;
    userData[receiverAccountNumber] = receiverAccount;

    // Write the updated data back to the DataBase.json file
    writeJsonFile("../../../Database/DataBase.json", userData);

    // Log the successful transaction
    qDebug() << "Transfer Successful: New balance for sender account" << senderAccountNumber << "is" << senderBalance << ". New balance for receiver account" << receiverAccountNumber << "is" << receiverBalance;
    return "Transfer Successful";
}

QString ServerHandler::viewTransactionHistory(const QString &accountNumber, int count) {
    // Read the DataBase.json file to access user data
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");

    // Check if the account number exists in the database
    if (!userData.contains(accountNumber))
    {
        qWarning() << "Account number not found in database.";
        return "Can\'t send Transaction history, Account number not found in database.";
    }

    // Get the user's data from the database
    QJsonObject userAccount = userData.value(accountNumber).toObject();

    // Check if the Transaction History field exists for the account
    if (!userAccount.contains("Transaction History"))
    {
        qWarning() << "Transaction history not found for account number" << accountNumber;
        return "Can\'t send Transaction history, not found.";
    }

    // Get the transaction history as a QJsonArray
    QJsonArray transactionHistory = userAccount.value("Transaction History").toArray();

    // Extract transaction data into a vector of QPair<QDateTime, QString>
    std::vector<QPair<QDateTime, QString>> transactions;
    for (const QJsonValue &value : transactionHistory)
    {
        QJsonObject transaction = value.toObject();
        QDateTime date = QDateTime::fromString(transaction.value("Date").toString(), "yyyy-MM-dd HH:mm:ss");
        QString details = transaction.value("Details").toString();
        transactions.push_back(qMakePair(date, details));
    }

    // Sort the transactions in descending order by date
    std::sort(transactions.begin(), transactions.end(), [](const QPair<QDateTime, QString> &a, const QPair<QDateTime, QString> &b) {
        return a.first > b.first; // Sort in descending order
    });

    // Format the sorted transaction data as a string
    QString formattedHistory;
    QTextStream stream(&formattedHistory);

    stream << "Transaction History:\n";
    size_t countToDisplay = count; // Determine how many transactions to display
    for (size_t i = 0; i < countToDisplay && i < transactions.size(); ++i)
    {
        const auto &transaction = transactions[i];
        stream << "Date: " << transaction.first.toString("yyyy-MM-dd HH:mm:ss") << "\n";
        stream << "Details: " << transaction.second << "\n";
        stream << "-----------------------\n";
    }

    qDebug() << formattedHistory;

    return formattedHistory;
}

QString ServerHandler::viewBankDatabase()
{
    // Read the DataBase.json file to access user data
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");

    if (userData.isEmpty())
    {
        return "Can\'t send Database, not found."; // Return an empty string if the database is empty
    }

    QString databaseString;
    QTextStream stream(&databaseString);
    stream << "Database:\n";

    // Iterate over each user in the database
    for (auto it = userData.begin(); it != userData.end(); ++it)
    {
        // Get the account number (key) and the user data (value)
        QString accountNumber = it.key();
        QJsonObject user = it.value().toObject();

        // Append user details to the stream
        stream << "Username: " << user.value("Username").toString() << "\n";
        stream << "Account number: " << user.value("AccountNumber").toString() << "\n";
        stream << "Full name: " << user.value("Fullname").toString() << "\n";
        stream << "Balance: " << user.value("Balance").toInt() << "\n";
        stream << "Age: " << user.value("Age").toInt() << "\n";
        stream << "Email: " << user.value("Email").toString() << "\n";
        stream << "-----------------------\n"; // Separator for clarity
    }

    // Return the formatted database string
    return databaseString;
}

QString ServerHandler::encrypt(const QString &input)
{
    QString result;

    // Iterate through each character in the input string
    for (int i = 0; i < input.size(); ++i)
    {
        QChar currentChar = input[i];

        // Check if the current character is not a comma or a plus
        if (currentChar != ',' && currentChar != '+')
        {
            // Encrypt the character by shifting its Unicode value by +1
            result += QChar(currentChar.unicode() + 1);
        }
        else
        {
            // Keep the comma or + as it is
            result += currentChar;
        }
    }

    return result;
}

QString ServerHandler::decrypt(const QString &input)
{
    QString result;

    // Iterate through each character in the input string
    for (int i = 0; i < input.size(); ++i)
    {
        QChar currentChar = input[i];

        // Check if the current character is not a comma or a +
        if (currentChar != ',' && currentChar != '+')
        {
            // Decrypt the character by shifting its Unicode value by -1
            result += QChar(currentChar.unicode() - 1);
        }
        else
        {
            // Keep the comma or the plus as it is
            result += currentChar;
        }
    }

    return result;
}

void ServerHandler::sendEmail(const QString &sender, const QString &recipient, const QString &subject, const QString &body)
{
    // Path to the Python script that handles sending the email
    QString scriptPath = "../../../send_email1.py";

    // List of arguments to be passed to the Python script
    QStringList arguments;
    arguments << sender << recipient << subject << body;

    // Create a QProcess instance to run the Python script
    QProcess process;

    // Start the Python interpreter and pass the script path and arguments
    process.start("python", QStringList() << scriptPath << arguments);

    // Wait until the script finishes execution
    process.waitForFinished();

    // Read the standard output and error from the process
    QString output(process.readAllStandardOutput());
    QString error(process.readAllStandardError());

    // Check if there was any error during the script execution
    if (!error.isEmpty())
    {
        qWarning() << "Error:" << error; // Log the error message if any
    }
    else
    {
        qDebug() << "Output:" << output; // Log the output if the script ran successfully
    }
}

QString ServerHandler::getEmail(const QString &accountNumber)
{
    // Read the DataBase.json file to get the user data
    QJsonObject jsonObject = readJsonFile("../../../Database/DataBase.json");

    // Check if the account number exists in the JSON object
    if (jsonObject.contains(accountNumber))
    {
        // Retrieve the account object associated with the account number
        QJsonObject accountObject = jsonObject.value(accountNumber).toObject();

        // Check if the "Email" field exists in the account object
        if (accountObject.contains("Email"))
        {
            // Return the email address associated with the account number
            return accountObject.value("Email").toString();
        }
        else
        {
            // Log a warning if the "Email" field is not found
            qWarning() << "Email not found for account number:" << accountNumber;
        }
    }
    else
    {
        // Log a warning if the account number is not found in the database
        qWarning() << "Account number not found:" << accountNumber;
    }

    // Return an empty string if the email address or account number is not found
    return QString();
}

bool ServerHandler::doesAccountNumberExist(const QString &accountNumber)
{
    // Read the DataBase.json file
    QJsonObject userData = readJsonFile("../../../Database/DataBase.json");

    // Check if the account number exists in the JSON object
    if (userData.contains(accountNumber)) {
        return true; // Account number exists
    } else {
        return false; // Account number does not exist
    }
}


void ServerHandler::onReadyRead_slot()
{
    QFile logFile("../../../Log/server_log.txt");
    QTextStream logStream(&logFile);

    // Open the log file in append mode
    if (logFile.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Log file opened successfully.";
    }
    else
    {
        qWarning() << "Failed to open log file.";
    }

    // Read all data from the socket
    QByteArray byteArr = socket_ptr->readAll();
    QString str = QString(byteArr);

    // Log the received data before decryption
    qDebug() << "Before decryption:\nReceived Data: " << str << "From Client " << ID;

    // Split the received string into parts
    auto splitStr = str.split("/");
    if (splitStr.size() < 1 || splitStr[0].split(":").size() < 2) {
        qDebug() << "Invalid data format received from Client " << QString::number(ID);
        return;
    }

    // Extract the JSON size from the first part
    qint32 jsonSize = splitStr[0].split(":")[1].toInt();

    // Extract the JSON data part
    QString data = str.right(jsonSize);

    // Convert the extracted data into a QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data received from Client " << ID;
        return;
    }

    // Extract the JSON object from the document
    QJsonObject jsonObj = jsonDoc.object();

    // Extract data size, encrypted message, and request type from JSON
    qint32 dataSize = jsonObj["file_size"].toInt();
    QString encryptedMessage = jsonObj["file_data"].toString();
    QString realMessage = decrypt(encryptedMessage);
    QString encryptedRequest = jsonObj["file_type"].toString();
    QString request = decrypt(encryptedRequest);

    // Log the request and data after decryption
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString clientRequest = "Received Request: " + request + " with Data: " + realMessage + " From Client " + QString::number(ID);
    QString requestLog = QString("[%1] : %2").arg(currentTime).arg("Client " + QString::number(ID) + ", " + clientRequest);
    logStream << requestLog << Qt::endl;
    logStream.flush();  // Ensure the log is written immediately
    logFile.close();

    qDebug() << "Before decryption:\nReceived Request: " << encryptedRequest << " with Data: " << encryptedMessage << " From Client " << ID;
    qDebug() << "After decryption:\nReceived Request: " << request << " with Data: " << realMessage << " From Client " << ID;

    // Check if the size of the decrypted message matches the reported size
    if (dataSize == realMessage.size())
    {
        Operations(request + "," + realMessage);
    }
    else
    {
        qDebug() << "Data size mismatch for Client " << ID;
    }
}

void ServerHandler::onDisconnect_slot()
{
    // Check if the socket is still open
    if(socket_ptr->isOpen())
    {
        QFile logFile("../../../Log/server_log.txt");
        QTextStream logStream(&logFile);

        // Open the log file in append mode
        if (logFile.open(QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Log file opened successfully.";
        }
        else
        {
            qWarning() << "Failed to open log file.";
        }

        // Create a log entry for the disconnection event
        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString clientRequest = "Client " + QString::number(ID) + " disconnected from the server";
        QString requestLog = QString("[%1] : %2").arg(currentTime).arg(clientRequest);

        // Write the log entry to the file
        logStream << requestLog << Qt::endl;
        logStream.flush();  // Ensure the log is written immediately
        logFile.close();

        // Disconnect the socket
        socket_ptr->disconnect();
        qDebug() << "Client " << ID << " has been disconnected";
    }
}
