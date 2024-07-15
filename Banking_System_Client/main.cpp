#include "mainwindow.h" // Include the header file for the MainWindow class

#include <QApplication> // Include the QApplication header file for managing the application

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Create a QApplication object

    // Style sheet downloaded from QSS stock (https://qss-stock.devsecstudio.com/)
    QFile styleSheetFile("..\\..\\Style\\Darkeum.qss"); // Define a QFile object to read the stylesheet file

    if (!styleSheetFile.open(QIODevice::ReadOnly)) // Check if the stylesheet file can be opened
    {
        qWarning() << "Couldn't open users file."; // Output a warning message if the file couldn't be opened
    }
    QString styleSheet = QLatin1String(styleSheetFile.readAll()); // Read the contents of the stylesheet file into a QString

    a.setStyleSheet(styleSheet); // Set the application's stylesheet using the read stylesheet

    MainWindow w; // Create an instance of the MainWindow class

    w.setWindowTitle("Library Management System"); // Set the window title

    QIcon icon("../../icon/icon.ico"); // Define a QIcon object with the path to the icon file
    w.setWindowIcon(icon); // Set the window icon

    w.show(); // Show the main window

    return a.exec(); // Execute the application event loop and return the exit status
}
