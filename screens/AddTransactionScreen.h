#pragma once

#include <iostream>
#include <string>

#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showAddTransactionScreen() {
  clearScreen();

  drawBoxedTitle("Add Transaction");
  std::cout << std::endl;

  drawSeparator();
  std::cout << "  Select Type:" << std::endl;
  std::cout << "  ";
  setColor(12);
  std::cout << "[1] Expense";
  resetColor();
  std::cout << "   ";
  setColor(10);
  std::cout << "[2] Income";
  resetColor();
  std::cout << "   ";
  setColor(11);
  std::cout << "[3] AI Assistant";
  resetColor();
  std::cout << std::endl << std::endl;

  std::cout << "  Choice: ";
  std::string choiceStr = getInput();

  int choice = 0;
  try {
    choice = std::stoi(choiceStr);
  } catch (...) {
    choice = 0;
  }

  std::string type;
  if (choice == 1) {
    type = "expense";
  } else if (choice == 2) {
    type = "income";
  } else if (choice == 3) {
    showAIScreen();
    return;
  } else {
    setColor(12);
    std::cout << "✗ Invalid choice." << std::endl;
    resetColor();
    std::cout << std::endl;
    std::cout << "Press ENTER to retry..." << std::endl;
    std::cin.get();
    showAddTransactionScreen();
    return;
  }

  std::cout << "  Amount: $";
  double amount = getDoubleInput();
  if (amount <= 0) {
    setColor(12);
    std::cout << "✗ Invalid amount." << std::endl;
    resetColor();
    std::cout << std::endl;
    std::cout << "Press ENTER to retry..." << std::endl;
    std::cin.get();
    showAddTransactionScreen();
    return;
  }

  std::cout << "  Description: ";
  std::string description = getInput();
  if (description.empty()) {
    setColor(12);
    std::cout << "✗ Description cannot be empty." << std::endl;
    resetColor();
    std::cout << std::endl;
    std::cout << "Press ENTER to retry..." << std::endl;
    std::cin.get();
    showAddTransactionScreen();
    return;
  }

  std::cout << "  Note (optional): ";
  std::string note = getInput();
  if (!note.empty()) {
    description += " - " + note;
  }

  std::cout << std::endl;

  if (TransactionManager::addTransaction(type, amount, description)) {
    setColor(10);
    std::cout << "✓ ";
    if (type == "income") {
      std::cout << "Income";
    } else {
      std::cout << "Expense";
    }
    std::cout << " added successfully!" << std::endl;
    resetColor();
    std::cout << std::endl;
  } else {
    setColor(12);
    std::cout << "✗ Failed to add transaction." << std::endl;
    resetColor();
    std::cout << std::endl;
  }

  std::cout << "Press ";
  setColor(14);
  std::cout << "ENTER";
  resetColor();
  std::cout << " to return to menu..." << std::endl;
  std::cin.ignore();
  std::cin.get();
  showMainMenu();
}
