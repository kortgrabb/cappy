#include "manager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>

// Define the helper functions
void displayUsage() {
    std::cout << YELLOW << "Available commands:" << RESET << std::endl;
    std::cout << "  add <site> <password> : Add a new password entry" << std::endl;
    std::cout << "  get <site>            : Retrieve a password for a site" << std::endl;
    std::cout << "  list                  : List all stored sites and passwords" << std::endl;
    std::cout << "  remove <site>         : Remove a password entry" << std::endl;
    std::cout << "  exit                  : Quit the application" << std::endl;
}

void addPassword(PasswordManager& pm, std::istringstream& iss) {
    std::string site, password;
    if (iss >> site >> password) {
        pm.createPass(site, password);
    }
    else {
        std::cout << YELLOW << "Usage: add <site> <password>" << RESET << std::endl;
    }
}

void getPassword(PasswordManager& pm, std::istringstream& iss) {
    std::string site;
    if (iss >> site) {
        pm.getPassForSite(site);
    }
    else {
        std::cout << YELLOW << "Usage: get <site>" << RESET << std::endl;
    }
}

void removePassword(PasswordManager& pm, std::istringstream& iss) {
    std::string site;
    if (iss >> site) {
        pm.removePass(site);
    }
    else {
        std::cout << YELLOW << "Usage: remove <site>" << RESET << std::endl;
    }
}

void processCommand(PasswordManager& pm, const std::string& command, std::istringstream& iss) {
    std::unordered_map<std::string, std::function<void()>> commandMap;

    commandMap["add"] = [&]() { addPassword(pm, iss); };
    commandMap["get"] = [&]() { getPassword(pm, iss); };
    commandMap["list"] = [&]() { pm.listPasses(); };
    commandMap["remove"] = [&]() { removePassword(pm, iss); };

    auto commandFunction = commandMap.find(command);
    if (commandFunction != commandMap.end()) {
        commandFunction->second();
    }
    else {
        std::cout << RED << "Unknown command: " << command << RESET << std::endl;
        displayUsage();
    }
}

int main() {
    PasswordManager pm;

    std::cout << "Password Manager CLI" << std::endl;
    displayUsage();

    std::string line, command;
    while (true) {
        std::cout << GREEN << "> " << RESET;
        if (!std::getline(std::cin, line) || line == "exit") {
            break;
        }

        std::istringstream iss(line);
        if (!(iss >> command)) {
            std::cout << RED << "Invalid input. Please enter a command." << RESET << std::endl;
            continue;
        }

        processCommand(pm, command, iss);
    }

    std::cout << "Exiting Password Manager CLI." << std::endl;
    return 0;
}
