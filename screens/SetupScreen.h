#pragma once

#include <iostream>
#include <string>

#include "../modules/AuthManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showSetupScreen()
{
  clearScreen();

  drawBoxedTitle("AI Expense Manager • Setup");
  std::cout << std::endl;

  std::cout << "👋 Welcome! ";
  setColor(11);
  std::cout << "Let's";
  resetColor();
  std::cout << " set up your profile." << std::endl;
  std::cout << std::endl;

  std::cout << "Enter your name: ";
  std::string username = getInput();

  std::cout << "Create a password: ";
  std::string password = getPasswordInput();

  std::cout << "Confirm password: ";
  std::string confirmPassword = getPasswordInput();

  std::cout << std::endl;

  if (AuthManager::setupUser(username, password, confirmPassword))
  {
    setColor(10);
    std::cout << "✓ Profile created successfully!" << std::endl;
    resetColor();
    std::cout << std::endl;

    std::cout << "Next time, just enter your ";
    setColor(11);
    std::cout << "name";
    resetColor();
    std::cout << " + ";
    setColor(11);
    std::cout << "password";
    resetColor();
    std::cout << " to continue." << std::endl;
    std::cout << std::endl;

    std::cout << "Press ";
    setColor(14);
    std::cout << "ENTER";
    resetColor();
    std::cout << " to launch your dashboard..." << std::endl;

    std::cin.ignore();
    std::cin.get();

    showMainMenu();
  }
  else
  {
    setColor(12);
    std::cout << "✗ Passwords do not match. Please try again." << std::endl;
    resetColor();
    std::cout << std::endl;
    std::cout << "Press ENTER to retry..." << std::endl;
    std::cin.get();
    showSetupScreen();
  }
}

