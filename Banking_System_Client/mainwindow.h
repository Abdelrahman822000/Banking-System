#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Client.h>
#include <QFile>
#include <QMetaEnum>
#include <QMessageBox>
#include <QEventLoop>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void onConnectionDevice();
    void onDisconnectedDevice();
    void onErrorOccurredDevice(QAbstractSocket::SocketError socketError);
    void onStateChangedDevice(QAbstractSocket::SocketState socketState);
    void onReadyReadDevice(QString data);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_ConnectButton_clicked();

    void on_ExitButton_clicked();

    void on_ExitButton_2_clicked();

    void on_ExitButton_3_clicked();

    void on_LoginButton_clicked();

private:
    Ui::MainWindow *ui;
    Client client;
    bool loginConfirmed;
    bool AdminCheck;
    bool UserCheck;
    bool UsernameCheck;
    bool PasswordCheck;
};
#endif // MAINWINDOW_H
