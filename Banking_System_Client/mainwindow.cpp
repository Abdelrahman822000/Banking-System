#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&client,&Client::Connection,this,&MainWindow::onConnectionDevice);
    connect(&client,&Client::Disconnected,this,&MainWindow::onDisconnectedDevice);
    connect(&client,&Client::ErrorOccurred,this,&MainWindow::onErrorOccurredDevice);
    connect(&client,&Client::StateChanged,this,&MainWindow::onStateChangedDevice);
    connect(&client,&Client::ReadyRead,this,&MainWindow::onReadyReadDevice);

    loginConfirmed = false;
    AdminCheck = false;
    UserCheck = false;
    UsernameCheck = false;
    PasswordCheck = false;

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnectionDevice()
{

}

void MainWindow::onDisconnectedDevice()
{

}

void MainWindow::onErrorOccurredDevice(QAbstractSocket::SocketError socketError)
{
    QMetaEnum meta = QMetaEnum::fromType<QAbstractSocket::SocketError>();
}

void MainWindow::onStateChangedDevice(QAbstractSocket::SocketState socketState)
{
    QMetaEnum meta = QMetaEnum::fromType<QAbstractSocket::SocketState>();
}

void MainWindow::onReadyReadDevice(QString data)
{
    QString str = QString("Data Recieved => %1").arg(data);

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
        if(data.contains("Login Done as Admin"))
        {
            loginConfirmed = true;
            AdminCheck = true;
            UserCheck = false;
        }
        else if(data.contains("Login Done as User"))
        {
            loginConfirmed = true;
            AdminCheck = false;
            UserCheck = true;
        }
        else if(data.contains("Login Failed: Incorrect password."))
        {
            loginConfirmed = false;
            AdminCheck = false;
            UsernameCheck = true;
            PasswordCheck = false;
        }
        else if(data.contains("Login Failed: Username not found."))
        {
            loginConfirmed = false;
            AdminCheck = false;
            UsernameCheck = false;
            PasswordCheck = true;
        }
        else
        {
            //Do nothing
        }
    }
}

void MainWindow::on_ConnectButton_clicked()
{
    QString ip = client.getLocalIPAddresses(); /*ui->leIP->text();*/
    qint32 port = 1234; /*ui->lePort->text().toInt();*/

    client.ConnectToDevice(ip,port);

    QMessageBox::information(nullptr, "Connection Done", "Welcome.");

    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setCurrentIndex(1);
}


void MainWindow::on_ExitButton_clicked()
{
    client.Disconnect();

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setCurrentIndex(0);

    loginConfirmed = false;
    AdminCheck = false;
    UserCheck = false;
    UsernameCheck = false;
    PasswordCheck = false;
}


void MainWindow::on_ExitButton_2_clicked()
{
    client.Disconnect();

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setCurrentIndex(0);

    loginConfirmed = false;
    AdminCheck = false;
    UserCheck = false;
    UsernameCheck = false;
    PasswordCheck = false;
}


void MainWindow::on_ExitButton_3_clicked()
{
    client.Disconnect();

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setCurrentIndex(0);

    loginConfirmed = false;
    AdminCheck = false;
    UserCheck = false;
    UsernameCheck = false;
    PasswordCheck = false;
}


void MainWindow::on_LoginButton_clicked()
{
    QString UserName = ui->UserName->text();
    QString Password = ui->Password->text();
    QString Request = "Login";

    QEventLoop loop;

    // Connect the ReadyRead signal to quit the event loop
    connect(&client, &Client::ReadyRead, &loop, &QEventLoop::quit);

    client.SendRequest(Request, UserName + "," + Password);

    // Start the event loop and wait for ReadyRead signal
    loop.exec();

    if(loginConfirmed)
    {
        qDebug() << "Login done" << Qt::endl;
        if(AdminCheck)
        {
            qDebug() << "Admin done" << Qt::endl;
            ui->tabWidget->setTabEnabled(0, false);
            ui->tabWidget->setTabEnabled(1, false);
            ui->tabWidget->setTabEnabled(2, true);
            ui->tabWidget->setTabEnabled(3, false);
            ui->tabWidget->setCurrentIndex(2);
        }
        else if(UserCheck)
        {
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
        if(!UsernameCheck)
        {
            qDebug() << "Wrong Username" << Qt::endl;
        }
        else if(!PasswordCheck)
        {
            qDebug() << "Wrong Password" << Qt::endl;
        }

    }
}

