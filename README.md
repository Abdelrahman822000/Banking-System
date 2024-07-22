# Banking System

A client-server banking system application that handles user authentication, account management, transaction processing, and administrative operations.

## Table of Contents
- [Project Description](#project-description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)
- [Contact](#contact)

## Project Description

This project is a simple client-server banking system developed in C++ using the Qt framework. 
The system allows users to view their account balance, account number, transaction history, and perform transactions. 
Administrators can view the bank database, add, update, and delete user accounts. 
Can also view data of users such as transaction history, account balance, and account number.
The server handles multiple client connections and processes all requests from users such as login, transaction processing, and user management.

## Features

- User authentication
- User operations: View account balance, account number, transaction history, and Perform transactions
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
    - Open the Banking_System_Server in Qt Creator by choosing `file -> Open file or Project` then navigate to where you cloned the repository and choose `Banking_System_Server.pro`
    - Open the Banking_System_Client in Qt Creator by choosing `file -> Open file or Project` then navigate to where you cloned the repository and choose `Banking_System_Client.pro` 
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
    - Run the project using Qt creator the application will build then starts.
    - The server will start listening for incoming connections on port 1234.
    - Any time you want to run again you can either run it from Qt Creator again or navigate to `Banking_System_Server/build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/release` and run the `Banking_System_Server.exe` executable.

### Running the Client

1. **Start the client:**
    - Run the project using Qt creator the application will build then starts.
    - Connect to the server using the appropriate IP and port.
    - The client will set the IP to your local IP address and the port to 1234
    - Any time you want to run again you can either run it from Qt Creator again or navigate to `Banking_System_Client/build/Desktop_Qt_6_7_0_MinGW_64_bit-Release/release` and run the `Banking_System_Client.exe` executable.

2. **Client Operations:**
    1. ***User Operations:***
        - **Login:** Enter your credentials to log in.
        - **View Account balance:** View your account balance.
        - **View Account number:** View your account number.
        - **View Transaction History:** See your past transactions.
        - **Perform Transaction:** Deposit funds to your accounts, Withdraw funds from your account or Transfer funds to another account.

    2. **Admin Operations:** 
        - **Login:** Enter your credentials to log in.
        - **View Account balance:** View your account balance.
        - **View Account number:** View your account number.
        - **View Transaction History:** See your past transactions.
        - **View the bank database:** View the details of all users
        - **Add User:** Add a new user to the database.
        - **Update User:** Update the data of a user in the database.
        - **Delete user:** Delete a user from the database; 
### Logging

- All interactions are logged in `Log/server_log.txt`.
- All server responses will be sent to the email of the current logged in user.

## Contact

If you have any questions, feel free to contact me:
- **Phone:** +201028325749
- **Email:** a.mohamed822000@gmail.com
- **LinkedIn:** [Abdelrahman Mohamed Hamad](https://www.linkedin.com/in/abdelrahman-mohamed-a1956b247/)
