**This Project was developed by Abdelrahman Mohamed Hamad**

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
    - **Welcome page:** Connect to the server. ![Welcome](https://github.com/user-attachments/assets/48475698-8946-4513-be92-839f4a024d7b)

    - **Login:** Check the credentials in `Database/Login.json` ![Login page](https://github.com/user-attachments/assets/55d069cf-f630-4be7-9a00-11eb0c102768)

    1. **Admin Operations:**
        - **Add User:** Add a new user to the database. ![Create new user](https://github.com/user-attachments/assets/db4b07d6-ab8d-440c-bf8f-1fc04c69098b)

        - **Update User:** Update the data of a user in the database. ![Update User](https://github.com/user-attachments/assets/a6ba5b06-8beb-47af-9163-eb8e7a497790)

        - **Delete user:** Delete a user from the database. ![Delete User](https://github.com/user-attachments/assets/bf83ec1f-bd7a-42bd-b0ac-72b522d44242)

        - **View Account balance:** View account balance of a user. ![View Account Balance Admin](https://github.com/user-attachments/assets/d29ce41d-7102-4b49-9c72-7b1db5b8ba77)

        - **View Account number:** View account number of a user. ![Get Account Number Admin](https://github.com/user-attachments/assets/f0188e3b-409d-4ae9-925f-423dd4951b80)

        - **View Transaction History:** See past transactions of a user.

          ![View Transaction History Admin](https://github.com/user-attachments/assets/31dc7570-30bb-46c8-b0f3-0d8adfdd7d3f)

        - **View the bank database:** View the details of all users

          ![View Bank Database](https://github.com/user-attachments/assets/08446e97-db7f-419f-9884-962a810fc222)

        - **Output:**

          ![Output Admin](https://github.com/user-attachments/assets/bd017a98-1465-4fa1-8324-caad46db10f9)
          
    3. ***User Operations:***
        ![User Tab](https://github.com/user-attachments/assets/55d9f786-2a97-4622-8573-acfa0b39856d)

        - **View Account balance**

          ![View Account Balance User](https://github.com/user-attachments/assets/abfacea2-a196-43dc-9dee-70ba67387f1f)

        - **View Account number**

          ![Get Account Number User](https://github.com/user-attachments/assets/a80b9485-4aa4-4d73-8b7f-7504f690ec7a)

        - **View Transaction History**

          ![View Transaction History User](https://github.com/user-attachments/assets/44d7efa4-b6e6-410e-807a-46fe6f6f6324)

        - **Perform Transaction:** Deposit funds to your accounts, Withdraw funds from your account or Transfer funds to another account.
        ![Make Transaction](https://github.com/user-attachments/assets/69295986-aadf-43f9-af07-1db87f614e98)
        ![Transfer Amount](https://github.com/user-attachments/assets/269f6bc5-62c8-49b0-b629-84642e7ff5f9)

        - **Output:** ![Output User](https://github.com/user-attachments/assets/a140f52c-6948-4847-8537-ba0d1bcb5a51)

### Logging

- All interactions are logged in `Log/server_log.txt`.
- All server responses will be sent to the email of the current logged in user.

## License

This project is licensed under the  GNU GENERAL PUBLIC LICENSE - see the [LICENSE](LICENSE) file for details.

## Contact

If you have any questions, feel free to contact me:
- **Phone:** +201028325749
- **Email:** a.mohamed822000@gmail.com
- **LinkedIn:** [Abdelrahman Mohamed Hamad](https://www.linkedin.com/in/abdelrahman-mohamed-a1956b247/)
