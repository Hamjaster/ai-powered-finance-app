#pragma once

#include <iostream>
#include <string>
#include <windows.h>

#include "../modules/AuthManager.h"
#include "../modules/FileHandler.h"
#include "../modules/User.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showLoginScreen() {
  clearScreen();

  drawScreenHeader("AI Expense Manager - Login", false);
  std::cout << std::endl;
  
  drawThinBox({
    "Welcome back!",
    "",
    "Please enter your password to access your account.",
    "Your data is encrypted and secure."
  });
  
  std::cout << std::endl;
  drawPrompt("Password");
  std::string password = getPasswordInput();
  std::cout << std::endl;

  if (AuthManager::login(password)) {
    // Get username AFTER successful login
    User currentUser = AuthManager::getCurrentUser();
    std::string username = currentUser.getUsername();
    
    std::cout << std::endl;
    drawSuccessBox("Login successful! Welcome, " + username + "!");
    
    Sleep(1500);
    showMainMenu();
  } else {
    std::cout << std::endl;
    drawErrorBox("Invalid password. Please try again.");
    std::cout << std::endl;
    std::cout << "  Press ENTER to retry...";
    std::cin.get();
    showLoginScreen();
  }
}
