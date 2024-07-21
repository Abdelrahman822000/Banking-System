#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Client.h>
#include <QFile>
#include <QMetaEnum>
#include <QMessageBox>
#include <QEventLoop>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// MainWindow class definition
class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    // Slots for handling signals from the Client class
    void onConnectionDevice(); // Slot for handling successful connection
    void onDisconnectedDevice(); // Slot for handling disconnection
    void onReadyReadDevice(QString data); // Slot for handling data ready to be read

public:
    MainWindow(QWidget *parent = nullptr); // Constructor
    ~MainWindow(); // Destructor
    void showMessageBox(QString title, QString text, QMessageBox::Icon icon, QMessageBox::StandardButton button); // Utility function for showing message boxes

private slots:
    // Slots for handling various button clicks in the UI
    void on_ConnectButton_clicked(); // Slot for handling Connect button click
    void on_LoginButton_clicked(); // Slot for handling Login button click
    void on_CreateButton_clicked(); // Slot for handling Create button click
    void on_UpdateButton_clicked(); // Slot for handling Update button click
    void on_Delete_clicked(); // Slot for handling Delete button click
    void on_GetButton_clicked(); // Slot for handling Get button click
    void on_GetButton_2_clicked(); // Slot for handling second Get button click
    void on_ViewButton_clicked(); // Slot for handling View button click
    void on_ViewButton_2_clicked(); // Slot for handling second View button click
    void on_ViewButton_3_clicked(); // Slot for handling third View button click
    void on_ViewButton_5_clicked(); // Slot for handling fifth View button click
    void on_ViewButton_6_clicked(); // Slot for handling sixth View button click
    void on_Clear_clicked(); // Slot for handling Clear button click
    void on_Clear_2_clicked(); // Slot for handling second Clear button click
    void on_Clear_3_clicked(); // Slot for handling third Clear button click
    void on_Clear_4_clicked(); // Slot for handling fourth Clear button click
    void on_Clear_5_clicked(); // Slot for handling fifth Clear button click
    void on_ExitButton_clicked(); // Slot for handling Exit button click
    void on_ExitButton_2_clicked(); // Slot for handling second Exit button click
    void on_ExitButton_3_clicked(); // Slot for handling third Exit button click
    void on_StartButton_clicked(); // Slot for handling Start button click
    void on_StartButton_2_clicked(); // Slot for handling second Start button click

private:
    Ui::MainWindow *ui; // Pointer to the user interface
    QEventLoop loop; // Event loop for handling asynchronous events
    QMessageBox msgBox; // Message box for showing messages
    Client client; // Client instance for network communication
    void resetUI(); // Resets the user interface to its initial state
};

#endif // MAINWINDOW_H
