**This Project was developed by Abdelrahman Mohamed Hamad**

# Banking System Client

A client application for a banking system that handles user authentication, account management, transaction processing, and administrative operations.

## Table of Contents
- [Project Description](#project-description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Contact](#contact)

## Project Description

This project is a client application for a simple client-server banking system developed in C++ using the Qt framework. The client allows users to view their account balance, account number, transaction history, and perform transactions. Administrators can view the bank database, add, update, and delete user accounts. The client connects to the server to process all requests such as login, transaction processing, and user management.

## Features

- User authentication
- User operations: View account balance, account number, transaction history, and perform transactions
- Admin operations: View the bank database, data of users, add, update, and delete users 
- Log all client-server interactions
- Send email notifications for responses from the server

## Installation

### Prerequisites

- Qt Creator
- Qt 6.7.1 or higher
- C++ compiler (e.g., GCC, MinGW)

### Steps

1. **Clone the repository:**
    ```bash
    git clone https://github.com/Abdelrahman822000/Banking-System.git
    cd Banking-System
    ```

2. **Build the project:**
    - Open the Banking_System_Client in Qt Creator by choosing `file -> Open file or Project` then navigate to where you cloned the repository and choose `Banking_System_Client.pro`.
    - Configure the project using your desired kit.
    - Build the project using the Build menu.

## Usage

### Running the Client

1. **Start the client:**
    - Run the project using Qt Creator; the application will build and then start.
    - Connect to the server using the appropriate IP and port. The client will set the IP to your local IP address and the port to 1234.
    - Any time you want to run again, you can either run it from Qt Creator or navigate to `Banking_System_Client/build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/release` and run the `Banking_System_Client.exe` executable.

2. **Client Operations:**
    - **Welcome page:** Connect to the server.
    - **Login:** Check the credentials in `Database/Login.json`.
    - **User Operations:** View account balance, account number, transaction history, and perform transactions.
    - **Admin Operations:** Add, update, delete users, and view user data.

## License

This project is licensed under the GNU GENERAL PUBLIC LICENSE - see the [LICENSE](LICENSE) file for details.

## Contact

If you have any questions, feel free to contact me:
- **Phone:** +201028325749
- **Email:** a.mohamed822000@gmail.com
- **LinkedIn:** [Abdelrahman Mohamed Hamad](https://www.linkedin.com/in/abdelrahman-mohamed-a1956b247/)
