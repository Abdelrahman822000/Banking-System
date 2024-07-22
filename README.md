# Banking System

A client-server banking system application that handles user authentication, account management, transaction processing, and administrative operations.

## Table of Contents
- [Project Description](#project-description)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [File Structure](#file-structure)
- [License](#license)
- [Contributing](#contributing)
- [Contact](#contact)

## Project Description

This project is a simple client-server banking system developed in C++ using the Qt framework. The system allows users to log in, view their account details, view transaction history, and perform transactions. Administrators can add, update, and delete user accounts. The server handles multiple client connections and processes various requests such as login, transaction processing, and user management.

## Features

- User authentication
- View account details
- View transaction history
- Perform transactions
- Admin operations: add, update, and delete users
- Log all client-server interactions
- Send email notifications for transactions

## Installation

### Prerequisites

- Qt 6.7.1 or higher
- C++ compiler (e.g., GCC, MinGW)
- Python (for email sending script)

### Steps

1. **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/banking_system.git
    cd banking_system
    ```

2. **Build the project:**
    - Open the project in Qt Creator.
    - Configure the project using your desired kit.
    - Build the project using the Build menu.

3. **Prepare the Database:**
    - Ensure `DataBase.json` is placed in the `Database` directory.

4. **Prepare the Email Script:**
    - Ensure `send_email1.py` is placed in the root directory.

## Usage

### Running the Server

1. **Start the server:**
    - Run the `Banking_System_Server.exe` executable.
    - The server will start listening for incoming connections.

### Running the Client

1. **Start the client:**
    - Run the `Banking_System_Client.exe` executable.
    - Connect to the server using the appropriate IP and port.

2. **Client Operations:**
    - **Login:** Enter your credentials to log in.
    - **View Account Details:** View your account balance and other details.
    - **View Transaction History:** See your past transactions.
    - **Perform Transaction:** Transfer funds to another account.
    - **Admin Operations:** Add, update, or delete user accounts (admin only).

### Logging

- All interactions are logged in `Log/server_log.txt`.

## File Structure

