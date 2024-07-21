#include <QCoreApplication>  // Include the QCoreApplication class for creating a console application
#include <Server.h>          // Include the custom Server class header

int main(int argc, char *argv[])
{
    // Create a QCoreApplication object, which manages application-wide resources and settings
    QCoreApplication a(argc, argv);

    // Create an instance of the Server class
    Server server;

    // Start the server
    server.startServer();

    // Enter the event loop to wait for and dispatch events
    return a.exec();
}
