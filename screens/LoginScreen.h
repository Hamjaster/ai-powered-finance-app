#pragma once

#include <iostream>
#include <string>
#include <windows.h>

#include "../modules/AuthManager.h"
#include "../modules/FileHandler.h"
#include "../modules/User.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showLoginScreen()
{
  clearScreen();

  drawBoxedTitle("AI Expense Manager • Login");
  std::cout << std::endl;
  std::cout << "debuggin STARTS";

  User storedUser = FileHandler::readUserFromFile();
  std::string username = storedUser.getUsername();

  std::cout << "Hy, " << username << std::endl;
  std::cout << "Name: " << username << std::endl;

  std::cout << "Password: ";
  std::string password = getPasswordInput();

  std::cout << std::endl;

  if (AuthManager::login(password))
  {
    setColor(10);
    std::cout << "✓ Login successful. Loading your dashboard..." << std::endl;
    resetColor();

    Sleep(1500);

    showMainMenu();
  }
  else
  {
    setColor(12);
    std::cout << "✗ Invalid password. Please try again." << std::endl;
    resetColor();
    std::cout << std::endl;
    std::cout << "Press ENTER to retry..." << std::endl;
    std::cin.get();
    showLoginScreen();
  }
}

