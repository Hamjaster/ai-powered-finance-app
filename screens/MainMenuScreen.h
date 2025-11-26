#pragma once

#include <iomanip>
#include <iostream>
#include <string>

#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showMainMenu()
{
  clearScreen();

  drawBoxedTitle("AI Expense • Dashboard");
  std::cout << std::endl;

  double balance = TransactionManager::getBalance();
  double totalIncome = TransactionManager::getTotalIncome();
  double totalExpenses = TransactionManager::getTotalExpenses();

  std::cout << "💰 Balance: $";
  if (balance >= 0)
  {
    setColor(10);
  }
  else
  {
    setColor(12);
  }
  std::cout << std::fixed << std::setprecision(2) << balance << std::endl;
  resetColor();
  std::cout << std::endl;

  std::cout << "Commands:" << std::endl;
  std::cout << "  [";
  setColor(11);
  std::cout << "t";
  resetColor();
  std::cout << "]ransact - Add new transaction" << std::endl;
  std::cout << "  [";
  setColor(11);
  std::cout << "v";
  resetColor();
  std::cout << "]iew - View all transactions" << std::endl;
  std::cout << "  [";
  setColor(11);
  std::cout << "q";
  resetColor();
  std::cout << "]uit - Exit application" << std::endl;
  std::cout << std::endl;
  std::cout << "Enter command: ";

  std::string command = getInput();

  if (command == "t" || command == "T" || command == "transact")
  {
    showAddTransactionScreen();
  }
  else if (command == "v" || command == "V" || command == "view")
  {
    showViewTransactionsScreen();
  }
  else if (command == "q" || command == "Q" || command == "quit")
  {
    clearScreen();
    std::cout << "Thank you for using AI Expense Manager!" << std::endl;
    std::exit(0);
  }
  else
  {
    std::cout << "Invalid command. Press ENTER to continue..." << std::endl;
    std::cin.get();
    showMainMenu();
  }
}

