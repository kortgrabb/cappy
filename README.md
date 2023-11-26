# Password Manager CLI

## Overview
This Password Manager is a command-line interface (CLI) application written in C++ and using sqlite to store the data. It allows users to securely manage passwords for various sites with ease

## NOTE
This is just a fun project to test my skill. The data is only stored LOCALLY on your machine and you should not use this tool if you are not sure you wont lose the file, there are various other better tools than this one.

## Features
Below are the detailed explanations and examples for each command supported by the Password Manager:

### Add a New Password Entry
```bash
add <site> <password>
```
- Use this command to store a new password for a given site.
- Example: `add google.com mySecureP@ssw0rd` will store the password `mySecureP@ssw0rd` for the site `google.com`.
- Use the `-r` parameter on the password argument to generate a random 21 long password.

### Retrieve a Password
```bash
get <site>
```
- Retrieves the stored password for the specified site.
- Example: `get google.com` will display the password associated with `google.com`.
- The search matches partly too, meaning if you simply run `get google` it will still find it.

### List All Entries
```bash
list
```
- Lists all the sites along with their stored passwords.
- This is useful for getting an overview of all stored information.

### Remove an Entry
```bash
remove <site>
```
- Deletes the password entry for the specified site.
- Example: `remove google.com` will delete the stored password for `google.com`.

### Exit the Application
```bash
exit
```
- Use this command to safely exit the Password Manager CLI.

## Getting Started

### Prerequisites
- C++ compiler (e.g., GCC, Clang)
- Git (for cloning the repository)

### Installation
1. Clone the repository:
   ```bash
   git clone [https://github.com/yourusername/your-repository-name.git](https://github.com/kortgrabb/cappy.git)
   ```
2. Navigate to the cloned directory:
   ```bash
   cd cappy
   ```

### Building the Application
Run the following command in the root of the project directory or compile it in Visual Studio by opening the project file
```bash
g++ -o cappy main.cpp manager.cpp -std=c++11
```

This will compile the source files and create an executable named `cappy`.

### Running the Application
To start the Password Manager, run:
```bash
./cappy
```

## Contributing
Contributions to the project are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch for your feature (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a pull request.

## License
Distributed under the MIT License. See `LICENSE` for more information.

## Contact
Project Link: https://github.com/kortgrabb/cappy
