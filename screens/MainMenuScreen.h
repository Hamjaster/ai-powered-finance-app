#pragma once

#include <iomanip>
#include <iostream>
#include <string>

#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showMainMenu() {
  clearScreen();

  drawBoxedTitle("AI Expense • Dashboard");
  std::cout << std::endl;

  double balance = TransactionManager::getBalance();
  double totalIncome = TransactionManager::getTotalIncome();
  double totalExpenses = TransactionManager::getTotalExpenses();

  drawSeparator();
  std::cout << "  💰 Current Balance: ";
  if (balance >= 0) {
    setColor(10); // Green
    std::cout << "$";
  } else {
    setColor(12); // Red
    std::cout << "-$";
    balance = -balance;
  }
  std::cout << std::fixed << std::setprecision(2) << balance << std::endl;
  resetColor();
  drawSeparator();
  std::cout << std::endl;

  std::cout << "  Menu Options:" << std::endl;
  std::cout << std::endl;

  std::cout << "  [";
  setColor(11);
  std::cout << "t";
  resetColor();
  std::cout << "] Add Transaction" << std::endl;

  std::cout << "  [";
  setColor(11);
  std::cout << "v";
  resetColor();
  std::cout << "] View History" << std::endl;

  std::cout << "  [";
  setColor(11);
  std::cout << "a";
  resetColor();
  std::cout << "] AI Assistant" << std::endl;

  std::cout << "  [";
  setColor(11);
  std::cout << "q";
  resetColor();
  std::cout << "] Quit" << std::endl;

  std::cout << std::endl;
  drawSeparator();
  std::cout << "Enter command: ";

  std::string command = getInput();

  if (command == "t" || command == "T" || command == "transact") {
    showAddTransactionScreen();
  } else if (command == "v" || command == "V" || command == "view") {
    showViewTransactionsScreen();
  } else if (command == "q" || command == "Q" || command == "quit") {
    clearScreen();
    std::cout << "Thank you for using AI Expense Manager!" << std::endl;
    std::exit(0);
  } else if (command == "a" || command == "A" || command == "ai") {
    showAIScreen();
  } else {
    std::cout << "Invalid command. Press ENTER to continue..." << std::endl;
    std::cin.get();
    showMainMenu();
  }
}
