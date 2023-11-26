#include "manager.h"
#include <iostream>
#include <iomanip>
#include <random>

std::string generateRandomString(size_t length) {
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::string SPECIAL_CHARACTERS = "!@#$%^&*()-_=+[]{};:,.<>?";
    const std::string UPPERCASE_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> char_distribution(0, CHARACTERS.size() - 1);
    std::uniform_int_distribution<> special_char_distribution(0, SPECIAL_CHARACTERS.size() - 1);
    std::uniform_int_distribution<> uppercase_distribution(0, UPPERCASE_LETTERS.size() - 1);

    std::string random_string;

    // Ensure at least one special character and one uppercase letter
    random_string += SPECIAL_CHARACTERS[special_char_distribution(generator)];
    random_string += UPPERCASE_LETTERS[uppercase_distribution(generator)];

    // Fill the rest of the string with random characters
    for (size_t i = 2; i < length; ++i) {
        random_string += CHARACTERS[char_distribution(generator)];
    }

    // Shuffle to randomize the placement of the special character and uppercase letter
    std::shuffle(random_string.begin(), random_string.end(), generator);

    return random_string;
}


PasswordManager::PasswordManager() 
{
	if (sqlite3_open("passwordManager.db", &db)) {
		std::cerr << "Error opening database.\n";
	}
	else {
        const char* createTableQuery = R"(
            CREATE TABLE IF NOT EXISTS passwords (
                site TEXT PRIMARY KEY, 
                password TEXT NOT NULL
            );
        )";

        char* errMsg;
        if (sqlite3_exec(db, createTableQuery, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "Error creating table: " << errMsg << "\n";
            sqlite3_free(errMsg);
        }
        else {
            std::cout << "Table created successfully\n";
        }
	}
}

PasswordManager::~PasswordManager() 
{
	sqlite3_close(db);
}

void PasswordManager::createPass(const std::string& site, std::string& password)
{
    // Check if the password starts with "-r="
    if (password == "-r") 
    {
        password = generateRandomString(21);
    } 
    else if (password.size() > 3 && password.substr(0, 3) == "-r=")
    {
        std::string lengthStr = password.substr(3);

        // Check if the substring is all ldigits
        if (std::find_if(lengthStr.begin(), lengthStr.end(), [](char c) { return !std::isdigit(c); }) == lengthStr.end())
        {
            // Convert the length string to an integer
            int length = std::stoi(lengthStr);

            // Generate a random string of the specified length
            password = generateRandomString(length);
        }
        else {
            std::cerr << "Invalid length for random password generation.\n";
            return;
        }
    }

    std::string insertQuery = "INSERT INTO passwords (site, password) VALUES ('" + site + "', '" + password + "');";
    char* errorMessage;
    if (sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << RED << "Error inserting password: " << errorMessage << RESET << "\n";
        sqlite3_free(errorMessage);
    }
    else {
        std::cout << "Password " << GREEN << password << RESET << " added for " << GREEN << site << RESET << "\n";
    }
}

void PasswordManager::removePass(const std::string& site)
{
    std::string deleteQuery = "DELETE FROM passwords WHERE site = '" + site + "'";
    char* errorMessage;

    if (sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error deleting password: " << errorMessage << "\n";
        sqlite3_free(errorMessage);
    }
    else {
        std::cout << "Password deleted for " << site << "\n";
    }
}


void PasswordManager::getPassForSite(const std::string& partialSite)
{
    std::string fetchQuery = "SELECT site, password FROM passwords WHERE site LIKE ?";
    sqlite3_stmt* stmt;
    char* errorMessage;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, fetchQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    // Bind the partial site to the query
    std::string likePattern = "%" + partialSite + "%";
    if (sqlite3_bind_text(stmt, 1, likePattern.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        std::cerr << "Error binding parameter: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        return;
    }

    // Execute the query and process the results
    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = true;
        const char* site = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::cout << "Password for " << BLUE << site << RESET << ": " << GREEN << password << RESET << "\n";
    }

    if (!found) {
        std::cout << "No password for " << partialSite << " was found" << "\n";
    }

    // Clean up
    sqlite3_finalize(stmt);
}

void PasswordManager::listPasses()
{
    std::string selectQuery = "SELECT site, password FROM passwords;";
    char* errorMessage;
    std::cout << YELLOW << std::left << std::setw(20) << "Site" << std::setw(20) << "Password" << RESET << std::endl;
    std::cout << YELLOW << std::string(40, '-') << RESET << std::endl; // Creates a divider of '-' characters
    if (sqlite3_exec(db, selectQuery.c_str(), [](void*, int argc, char** argv, char** azColName) -> int {
        for (int i = 0; i < argc; i += 2) {
            std::cout << GREEN << std::left << std::setw(20) << (argv[i] ? argv[i] : "NULL") << RESET;
            std::cout << BLUE << std::setw(20) << (argv[i + 1] ? argv[i + 1] : "NULL") << RESET << std::endl;
        }
        return 0;
        }, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << RED << "Error listing passwords: " << errorMessage << RESET << "\n";
        sqlite3_free(errorMessage);
    }
    else {
        std::cout << YELLOW << std::string(40, '-') << RESET << std::endl; // Divider at the end
    }
}
