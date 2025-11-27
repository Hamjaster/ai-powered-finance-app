#include <iostream>
#include <windows.h> // For Windows console functions - must be FIRST

#include "modules/AuthManager.h"
#include "modules/FileHandler.h"
#include "screens/Screens.h"

int main() {
  // Set console output to UTF-8 for proper emoji display
  // Source - https://stackoverflow.com/a
  // Posted by SeanTolstoyevski
  // Retrieved 2025-11-27, License - CC BY-SA 4.0
  SetConsoleOutputCP(CP_UTF8);
  
  std::cout << "Starting AI Expense Manager...\n";
  FileHandler::ensureDataDirectory();

  if (AuthManager::isFirstTime()) {
    std::cout << "User is coming first time";
    showSetupScreen();
  } else {
    std::cout << "User aint first time";
    showLoginScreen();
  }

  return 0;
}
