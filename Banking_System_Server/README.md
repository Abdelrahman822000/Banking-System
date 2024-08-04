**This Project was developed by Abdelrahman Mohamed Hamad**

# Banking System Server

A server application for a banking system that handles user authentication, account management, transaction processing, and administrative operations.

## Table of Contents
- [Project Description](#project-description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Contact](#contact)

## Project Description

This project is a server application for a simple client-server banking system developed in C++ using the Qt framework. The server handles multiple client connections and processes all requests from users such as login, transaction processing, and user management. Administrators can view the bank database, add, update, and delete user accounts.

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
- Python (for email sending script)

### Steps

1. **Clone the repository:**
    ```bash
    git clone https://github.com/Abdelrahman822000/Banking-System.git
    cd Banking-System
    ```

2. **Build the project:**
    - Open the Banking_System_Server in Qt Creator by choosing `file -> Open file or Project` then navigate to where you cloned the repository and choose `Banking_System_Server.pro`.
    - Configure the project using your desired kit.
    - Build the project using the Build menu.

3. **Prepare the Database:**
    - Ensure `DataBase.json` is placed in the `Database` directory.
    - Ensure `Login.json` is placed in the `Database` directory.

4. **Prepare the Email Script:**
    - Ensure `send_email1.py` is placed in the root directory.

## Usage

### Running the Server

1. **Start the server:**
    - Run the project using Qt Creator; the application will build and then start.
    - The server will start listening for incoming connections on port 1234.
    - Any time you want to run again, you can either run it from Qt Creator or navigate to `Banking_System_Server/build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/release` and run the `Banking_System_Server.exe` executable.

### Logging

- All interactions are logged in `Log/server_log.txt`.
- All server responses will be sent to the email of the currently logged-in user.

## License

This project is licensed under the GNU GENERAL PUBLIC LICENSE - see the [LICENSE](LICENSE) file for details.

## Contact

If you have any questions, feel free to contact me:
- **Phone:** +201028325749
- **Email:** a.mohamed822000@gmail.com
- **LinkedIn:** [Abdelrahman Mohamed Hamad](https://www.linkedin.com/in/abdelrahman-mohamed-a1956b247/)
