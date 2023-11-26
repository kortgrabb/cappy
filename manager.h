#pragma once

#include <string>
#include "sqlite/sqlite3.h"

const std::string RED = "\033[31m";      // Red for errors
const std::string GREEN = "\033[32m";    // Green for success
const std::string YELLOW = "\033[33m";   // Yellow for warnings
const std::string BLUE = "\033[34m";     // Blue for information
const std::string MAGENTA = "\033[35m";  // Magenta for special messages
const std::string CYAN = "\033[36m";     // Cyan for prompts
const std::string WHITE = "\033[37m";    // White for general text
const std::string RESET = "\033[0m";     // Reset to default color

// Bright versions
const std::string BRIGHT_RED = "\033[91m";    // Bright Red
const std::string BRIGHT_GREEN = "\033[92m";  // Bright Green
const std::string BRIGHT_YELLOW = "\033[93m"; // Bright Yellow
const std::string BRIGHT_BLUE = "\033[94m";   // Bright Blue
const std::string BRIGHT_MAGENTA = "\033[95m";// Bright Magenta
const std::string BRIGHT_CYAN = "\033[96m";   // Bright Cyan
const std::string BRIGHT_WHITE = "\033[97m";  // Bright White


class PasswordManager
{
private:
	sqlite3* db;

public:
	PasswordManager();
	~PasswordManager();

	void createPass(const std::string& site, std::string& password);
	void removePass(const std::string& site);
	
	void getPassForSite(const std::string& site);
	void listPasses();
};