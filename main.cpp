#include <iostream>
#include <windows.h> // For Windows console functions - must be FIRST

#include "modules/AuthManager.h"
#include "modules/FileHandler.h"
#include "screens/Screens.h"

int main() {
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
