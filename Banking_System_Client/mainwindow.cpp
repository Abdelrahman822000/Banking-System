#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load and scale the logo image, then set it to label_2
    QPixmap Pix("../../../Logos/Logo.jpg");

    if (Pix.isNull())
    {
        qWarning() << "Failed to load the image"; // Log a warning if the image fails to load
    }
    else
    {
        qDebug() << "Image loaded successfully"; // Log success if the image loads correctly
    }

    int w = ui->label_2->width(); // Get the width of label_2
    int h = ui->label_2->height(); // Get the height of label_2
    ui->label_2->setPixmap(Pix.scaled(w, h, Qt::KeepAspectRatio)); // Scale and set the image to label_2

    // Connect signals from the client to the appropriate slots
    connect(&client, &Client::Connection, this, &MainWindow::onConnectionDevice);
    connect(&client, &Client::Disconnected, this, &MainWindow::onDisconnectedDevice);
    connect(&client, &Client::ReadyRead, this, &MainWindow::onReadyReadDevice);

    // Connect the ReadyRead signal to quit the event loop
    connect(&client, &Client::ReadyRead, &loop, &QEventLoop::quit);

    // Set minimum size for message box
    msgBox.setMinimumWidth(900);  // Set a minimum width for the message box
    msgBox.setMinimumHeight(300); // Set a minimum height for the message box

    // Initialize the tab widget, enabling only the first tab
    ui->tabWidget->setTabEnabled(0, true);  // Enable the Connect tab
    ui->tabWidget->setTabEnabled(1, false); // Disable the Login tab
    ui->tabWidget->setTabEnabled(2, false); // Disable the Admin tab
    ui->tabWidget->setTabEnabled(3, false); // Disable the User tab
    ui->tabWidget->setCurrentIndex(0);  // Set the current tab to Connect
}

MainWindow::~MainWindow()
{
    delete ui; // Clean up UI resources
}

void MainWindow::showMessageBox(QString title, QString text, QMessageBox::Icon icon, QMessageBox::StandardButton button)
{
    msgBox.setWindowTitle(title);        // Set the title of the message box
    msgBox.setText(text);                // Set the text of the message box
    msgBox.setIcon(icon);                // Set the icon of the message box
    msgBox.setStandardButtons(button);  // Set the standard buttons of the message box
    msgBox.adjustSize();                 // Adjust the size of the message box
    msgBox.exec();                       // Show the message box
}

// Resets the user interface to its initial state
void MainWindow::resetUI()
{
    // Enable the Connect tab and disable all others
    ui->tabWidget->setTabEnabled(0, true);  // Enable the Connect tab
    ui->tabWidget->setTabEnabled(1, false); // Disable the Login tab
    ui->tabWidget->setTabEnabled(2, false); // Disable the Admin tab
    ui->tabWidget->setTabEnabled(3, false); // Disable the User tab

    // Set the current tab to the Connect tab
    ui->tabWidget->setCurrentIndex(0);  // Show the Connect tab as the current view
}

void MainWindow::onConnectionDevice()
{
    // Show a message box when a connection is established
    showMessageBox("Connection Done", "Welcome to Banking System.", QMessageBox::Information, QMessageBox::Ok);

    // Update tab widget to show the Login tab
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setCurrentIndex(1); // Set current tab to Login
}

void MainWindow::onDisconnectedDevice()
{
    // Show a message box when the device is disconnected
    showMessageBox("Disconnected", "Thank you for banking with us!", QMessageBox::Information, QMessageBox::Ok);
}

void MainWindow::onReadyReadDevice(QString data)
{
    // Prepare the data string for output
    QString str = QString("Data Recieved => %1").arg(data);
    qDebug() << str << Qt::endl;
    // Add the received data to the appropriate output widget based on the current tab
    if(ui->tabWidget->currentIndex() == 2)
    {
        ui->OutputAdmin->addItem(str);
    }
    else if(ui->tabWidget->currentIndex() == 3)
    {
        ui->OutputUser->addItem(str);
    }
    else
    {
        // Do Nothing
    }

    // Process the received data for specific messages and update client state accordingly
    if(data.contains("Login Done as Admin"))
    {
        qDebug() << "Login" << Qt::endl;

        client.loginConfirmed = true;
        client.AdminCheck = true;
        client.UserCheck = false;
    }
    else if(data.contains("Can\'t send Database, not found."))
    {
        qDebug() << "Can\'t send Database" << Qt::endl;
        client.databaseCheck = false;
    }
    else if(data.contains("Database"))
    {
        qDebug() << "Database" << Qt::endl;
        client.databaseCheck = true;
        client.database = data;
    }
    else if(data.contains("Login Done as User"))
    {
        qDebug() << "Login done" << Qt::endl;

        client.loginConfirmed = true;
        client.AdminCheck = false;
        client.UserCheck = true;
    }
    else if(data.contains("Login Failed: Incorrect password."))
    {
        qDebug() << "Login failed" << Qt::endl;
        client.loginConfirmed = false;
        client.AdminCheck = false;
        client.UsernameCheck = true;
        client.PasswordCheck = false;
    }
    else if(data.contains("Login Failed: Username not found."))
    {
        qDebug() << "Login failed" << Qt::endl;
        client.loginConfirmed = false;
        client.AdminCheck = false;
        client.UsernameCheck = false;
        client.PasswordCheck = true;
    }
    else if(data.contains("New user added successfully."))
    {
        qDebug() << "New user added" << Qt::endl;
        client.newUserCheck = true;
    }
    else if(data.contains("Can\'t create new user, Username is not available."))
    {
        qDebug() << "User not added" << Qt::endl;
        client.newUserCheck = false;
    }
    else if(data.contains("User updated successfully."))
    {
        qDebug() << "User updated" << Qt::endl;
        client.updateUserCheck = true;
    }
    else if(data.contains("Can\'t update user, User not found."))
    {
        qDebug() << "User not updated" << Qt::endl;
        client.updateUserCheck = false;
    }
    else if(data.contains("User deleted successfully."))
    {
        qDebug() << "User deleted" << Qt::endl;
        client.deletedUserCheck = true;
    }
    else if(data.contains("Can\'t delete user, User not found."))
    {
        qDebug() << "User not deleted" << Qt::endl;
        client.deletedUserCheck = false;
    }
    else if(data.contains("Account number:"))
    {
        qDebug() << "Get Account number done" << Qt::endl;
        client.accountNumberCheck = true;
        client.accountNumber = data.split(":")[1];
    }
    else if(data.contains("Can\'t send account number, User not found."))
    {
        qDebug() << "Get Account number failed" << Qt::endl;
        client.accountNumberCheck = false;
        client.accountNumber = QString();
    }
    else if(data.contains("Transaction Done."))
    {
        qDebug() << "Transaction done" << Qt::endl;
        client.makeTransactionCheck = true;
    }
    else if(data.contains("Transaction failed, Balance insufficient."))
    {
        qDebug() << "Transaction failed" << Qt::endl;
        client.makeTransactionCheck = false;
    }
    else if(data.contains("Can\'t send account balance, Account number not found"))
    {
        qDebug() << "Can\'t send account balance" << Qt::endl;
        client.balance = -1;
    }
    else if(data.contains("Balance:"))
    {
        qDebug() << "Balance" << Qt::endl;
        client.balance = data.split(":")[1].toInt();
    }
    else if(data.contains("Transfer Successful."))
    {
        qDebug() << "Transfer Successful" << Qt::endl;
        client.transferCheck = true;
    }
    else if(data.contains("Transfer Failed"))
    {
        qDebug() << "Transfer Failed" << Qt::endl;
        client.transferCheck = false;
        client.transferMessage = data.split(":")[1];
    }
    else if(data.contains("Can\'t send Transaction history, Account number not found in database."))
    {
        qDebug() << "Can\'t send Transaction history" << Qt::endl;
        client.transactionHistory = "Account number not found in database.";
    }
    else if(data.contains("Can\'t send Transaction history, not found."))
    {
        qDebug() << "Can\'t send Transaction history" << Qt::endl;
        client.transactionHistory = "Transaction history not found.";
    }
    else if(data.contains("Transaction History"))
    {
        qDebug() << "Transaction history sent" << Qt::endl;
        client.transactionHistory = data;
    }
    else
    {
        qWarning() << "Wrong data format";
    }
}

void MainWindow::on_ConnectButton_clicked()
{
    QString ip = client.getLocalIPAddresses(); /*ui->leIP->text();*/
    qint32 port = 1234; /*ui->lePort->text().toInt();*/

    client.ConnectToDevice(ip,port);
}

void MainWindow::on_LoginButton_clicked()
{
    QString UserName = ui->UserName->text();
    QString Password = ui->Password->text();
    QString Request = "Login";

    if (UserName.isEmpty() || Password.isEmpty())
    {
        showMessageBox("Invalid Input", "Please fill all the fields correctly.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }

    QString data = UserName + "," + Password;
    client.SendRequest(Request, data);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.loginConfirmed)
    {
        qDebug() << "Login done" << Qt::endl;

        client.currentUser = UserName;

        if(client.AdminCheck)
        {
            showMessageBox("Login done", "Logged in as Admin.", QMessageBox::Information, QMessageBox::Ok);
            qDebug() << "Admin done" << Qt::endl;
            ui->tabWidget->setTabEnabled(0, false);
            ui->tabWidget->setTabEnabled(1, false);
            ui->tabWidget->setTabEnabled(2, true);
            ui->tabWidget->setTabEnabled(3, false);
            ui->tabWidget->setCurrentIndex(2);

            ui->tabWidget_2->setCurrentIndex(0);
        }
        else if(client.UserCheck)
        {
            showMessageBox("Login done", "Logged in as User.", QMessageBox::Information, QMessageBox::Ok);
            qDebug() << "User done" << Qt::endl;
            ui->tabWidget->setTabEnabled(0, false);
            ui->tabWidget->setTabEnabled(1, false);
            ui->tabWidget->setTabEnabled(2, false);
            ui->tabWidget->setTabEnabled(3, true);
            ui->tabWidget->setCurrentIndex(3);
        }

    }
    else
    {
        qDebug() << "Login failed" << Qt::endl;
        if(!client.UsernameCheck)
        {
            showMessageBox("Login failed", "Wrong Username.", QMessageBox::Critical, QMessageBox::Ok);
            qDebug() << "Wrong Username" << Qt::endl;
        }
        else if(!client.PasswordCheck)
        {
            showMessageBox("Login failed", "Wrong Password.", QMessageBox::Critical, QMessageBox::Ok);
            qDebug() << "Wrong Password" << Qt::endl;
        }

    }
}

void MainWindow::on_CreateButton_clicked()
{
    QString FullName = ui->FullName->text();
    QString Username = FullName.split(" ")[0].toLower();
    QString Password = ui->Password_2->text();
    QString Email = ui->Email->text();
    QString Age = ui->Age->text();
    QString Signature = ui->Signature->text();

    if (FullName.isEmpty() || Username.isEmpty() || Password.isEmpty() || Email.isEmpty() || (!Age.isEmpty() &&(Age.toInt() <= 0 || Age.toInt() > 100)) || Age.isEmpty() || Signature.isEmpty())
    {
        showMessageBox("Invalid Input", "Please fill all the fields correctly.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    else
    {
        QString Request = "Create User";

        QString data = Username + "," + Password + "," + FullName + "," + Email + "," + Age + "," + Signature;

        client.SendRequest(Request, data);

        // Start the event loop and wait for ReadyRead signal
        loop.exec();

        if(client.newUserCheck)
        {
            showMessageBox("User Added", "New user added successfully.", QMessageBox::Information, QMessageBox::Ok);
        }
        else
        {
            showMessageBox("User Not Added", "Username is not available.", QMessageBox::Warning, QMessageBox::Ok);
        }
    }

}

void MainWindow::on_UpdateButton_clicked()
{
    QString FullName = ui->FullName_2->text();
    QString Username = FullName.split(" ")[0].toLower();
    QString Password = ui->Password_3->text();
    QString Email = ui->Email_2->text();
    QString Age = ui->Age_2->text();
    QString AccountNumber = ui->AccountNumber_3->text();
    QString Signature = ui->Signature_3->text();

    if (AccountNumber.isEmpty())
    {
        showMessageBox("Invalid Input", "You didn\'t enter the account number.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    else if((!Age.isEmpty() &&(Age.toInt() <= 0 || Age.toInt() > 100)))
    {
        showMessageBox("Invalid Input", "You entered an invalid age.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    else if(FullName.isEmpty() && Username.isEmpty() && Password.isEmpty() && Email.isEmpty() && Signature.isEmpty() && Age.isEmpty())
    {
        showMessageBox("Invalid Input", "You didn\'t enter any data.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    else
    {
        QString Request = "Update User";

        QString data = Username + "," + Password + "," + FullName + "," + Email + "," + Age + "," + AccountNumber + "," + Signature;
        client.SendRequest(Request, data);

        // Start the event loop and wait for ReadyRead signal
        loop.exec();

        if(client.updateUserCheck)
        {
            showMessageBox("User updated", "User updated successfully.", QMessageBox::Information, QMessageBox::Ok);
        }
        else
        {
            showMessageBox("User Not updated", "User not found.", QMessageBox::Warning, QMessageBox::Ok);
        }
    }


}

void MainWindow::on_Delete_clicked()
{
    QString AccountNumber = ui->AccountNumber_4->text();
    QString Request = "Delete User";

    if (AccountNumber.isEmpty())
    {
        showMessageBox("Invalid Input", "You didn\'t enter the account number.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    client.SendRequest(Request, AccountNumber);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.deletedUserCheck)
    {
        showMessageBox("User deleted", "User deleted successfully.", QMessageBox::Information, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("User Not deleted", "User not found.", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void MainWindow::on_GetButton_clicked()
{
    QString userName = ui->UserName_2->text();
    QString Request = "Get Account Number as admin";

    if (userName.isEmpty())
    {
        showMessageBox("Invalid Input", "You didn\'t enter username.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }

    client.SendRequest(Request, userName);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.accountNumberCheck)
    {
        showMessageBox("User Found", "Account number: " + client.accountNumber, QMessageBox::Information, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("User Not Found", "Couldn\'t get account number.", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void MainWindow::on_GetButton_2_clicked()
{
    QString userName = client.currentUser;
    QString Request = "Get Account Number as user";

    if (userName.isEmpty())
    {
        showMessageBox("Invalid Input", "Invalid username.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    client.SendRequest(Request, userName);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.accountNumberCheck)
    {
        showMessageBox("User Found", "Account number: " + client.accountNumber, QMessageBox::Information, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("User Not Found", "Couldn\'t get account number.", QMessageBox::Warning, QMessageBox::Ok);
    }
}

void MainWindow::on_ViewButton_clicked()
{
    QString accountNumber = ui->AccountNumber->text();
    QString count = ui->Count->text();

    if (accountNumber.isEmpty() || count.isEmpty())
    {
        showMessageBox("Invalid Input", "Please fill all the fields correctly.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    QString Request = "View Transaction History as admin";

    QString data = accountNumber + "," + count;
    client.SendRequest(Request, data);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.transactionHistory.contains("Can\'t send Transaction history"))
    {
        showMessageBox("Transaction History", client.transactionHistory, QMessageBox::Warning, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("Transaction History", client.transactionHistory, QMessageBox::Information, QMessageBox::Ok);
    }
}

void MainWindow::on_ViewButton_2_clicked()
{
    QString accountNumber = ui->AccountNumber_2->text();

    if (accountNumber.isEmpty())
    {
        showMessageBox("Invalid Input", "Please enter an account number", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }

    QString Request = "View Account Balance as admin";

    QString data = accountNumber;
    client.SendRequest(Request, data);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.balance == -1)
    {
        showMessageBox("Account Balance", "Account number not found", QMessageBox::Warning, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("Account Balance", "Balance: " + QString::number(client.balance), QMessageBox::Information, QMessageBox::Ok);
    }

}

void MainWindow::on_ViewButton_3_clicked()
{
    QString Request = "View Bank Database";
    client.SendRequest(Request,"None");

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    qDebug() << client.databaseCheck << Qt::endl;
    if(client.databaseCheck)
    {
        showMessageBox("Database", client.database, QMessageBox::Information, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("Database", "Database not found", QMessageBox::Information, QMessageBox::Ok);
    }
}

void MainWindow::on_ViewButton_5_clicked()
{
    QString user = client.currentUser;
    QString count = ui->Count_2->text();

    if (user.isEmpty() || count.isEmpty())
    {
        showMessageBox("Invalid Input", "Please fill all the fields correctly.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    QString Request = "View Transaction History as user";

    QString data = user + "," + count;
    client.SendRequest(Request, data);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.transactionHistory.contains("Can\'t send Transaction history"))
    {
        showMessageBox("Transaction History", client.transactionHistory, QMessageBox::Warning, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("Transaction History", client.transactionHistory, QMessageBox::Information, QMessageBox::Ok);
    }
}

void MainWindow::on_ViewButton_6_clicked()
{
    QString user = client.currentUser;

    if (user.isEmpty())
    {
        showMessageBox("Invalid Input", "Invalid username.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }
    QString Request = "View Account Balance as user";

    QString data = user;
    client.SendRequest(Request, data);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    showMessageBox("Account Balance", "Balance: " + QString::number(client.balance), QMessageBox::Information, QMessageBox::Ok);
}

void MainWindow::on_Clear_clicked()
{
    ui->OutputAdmin->clear();
}

void MainWindow::on_Clear_2_clicked()
{
    ui->OutputUser->clear();
}

void MainWindow::on_Clear_3_clicked()
{
    ui->FullName->clear();
    ui->Password_2->clear();
    ui->Email->clear();
    ui->Age->clear();
    ui->Signature->clear();
}

void MainWindow::on_Clear_4_clicked()
{
    ui->FullName_2->clear();
    ui->Password_3->clear();
    ui->Email_2->clear();
    ui->Age_2->clear();
    ui->AccountNumber_3->clear();
    ui->Signature_3->clear();
}

void MainWindow::on_Clear_5_clicked()
{
    ui->AccountNumber_4->clear();
}

void MainWindow::on_ExitButton_clicked()
{
    client.Disconnect();
    resetUI();
}


void MainWindow::on_ExitButton_2_clicked()
{
    client.Disconnect();
    resetUI();
}


void MainWindow::on_ExitButton_3_clicked()
{
    client.Disconnect();
    resetUI();
}

void MainWindow::on_StartButton_clicked()
{
    QString user = client.currentUser;
    QString recieverAccountNumber = ui->AccountNumberRx->text();
    QString amount = ui->Amount->text();
    QString signature = ui->Signature_2->text();

    if (user.isEmpty() || recieverAccountNumber.isEmpty() || amount.isEmpty() || signature.isEmpty())
    {
        showMessageBox("Invalid Input", "Please fill all the fields correctly.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }

    QString Request = "Transfer Amount";

    QString data = user + "," + recieverAccountNumber + "," + amount + "," + signature;
    client.SendRequest(Request, data);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.transferCheck)
    {

        showMessageBox("Transfer Done", "Amount Transfered Succesfully" , QMessageBox::Information, QMessageBox::Ok);
    }
    else
    {
        showMessageBox("Transfer failed", client.transferMessage, QMessageBox::Warning, QMessageBox::Ok);
    }
}

void MainWindow::on_StartButton_2_clicked()
{
    QString user = client.currentUser;
    QString amount = ui->Amount_2->text();
    QString operation = ui->Operation->currentText();

    if (user.isEmpty() || amount.isEmpty() || operation.isEmpty())
    {
        showMessageBox("Invalid Input", "Please fill all the fields correctly.", QMessageBox::Warning, QMessageBox::Ok);
        return;
    }

    QString Request = "Make Transaction";

    QString data = user + "," + amount + "," + operation;
    client.SendRequest(Request, data);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(client.makeTransactionCheck)
    {
        if(operation == "Deposit")
        {
            showMessageBox("Transaction Done", "Amount depositted.", QMessageBox::Information, QMessageBox::Ok);
        }
        else if (operation == "Withdraw")
        {
            showMessageBox("Transaction Done", "Amount withdrawed.", QMessageBox::Information, QMessageBox::Ok);
        }
    }
    else
    {
        showMessageBox("Transaction failed", "Balance insufficient.", QMessageBox::Warning, QMessageBox::Ok);
    }
}
