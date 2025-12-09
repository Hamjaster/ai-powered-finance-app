#pragma once

#include <iostream>
#include <string>

#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showAddTransactionScreen() {
  clearScreen();

  // Beautiful header
  drawScreenHeader("AI Expense - Add Transaction", true);
  std::cout << std::endl;

  // Transaction type selection with icons
  drawInfoBox("                   What would you like to add?");
  std::cout << std::endl;

  drawMenuOption("1", "Add Expense", "💸");
  drawMenuOption("2", "Add Income", "💰");
  drawMenuOption("3", "AI Assistant", "🤖");
  std::cout << std::endl;
  drawMenuOption("b", "Go Back", "←");

  drawPrompt("Choice");
  std::string choiceStr = getInput();

  // Handle navigation
  if (choiceStr == "b" || choiceStr == "B" || choiceStr == "back") {
    showMainMenu();
    return;
  }
  if (handleNavigation(choiceStr)) return;

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
    std::cout << std::endl;
    drawStatusMessage("Invalid choice. Please try again.", "error");
    std::cout << std::endl;
    std::cout << "  Press ENTER to retry...";
    std::cin.get();
    showAddTransactionScreen();
    return;
  }

  // Amount input
  std::cout << std::endl;
  if (type == "expense") {
    drawSectionTitle("New Expense", "💸");
  } else {
    drawSectionTitle("New Income", "💰");
  }

  std::cout << "  Amount: $";
  double amount = getDoubleInput();
  if (amount <= 0) {
    std::cout << std::endl;
    drawStatusMessage("Invalid amount. Must be greater than 0.", "error");
    std::cout << std::endl;
    std::cout << "  Press ENTER to retry...";
    std::cin.get();
    showAddTransactionScreen();
    return;
  }

  // Description input
  std::cout << "  Description: ";
  std::string description = getInput();
  if (description.empty()) {
    std::cout << std::endl;
    drawStatusMessage("Description cannot be empty.", "error");
    std::cout << std::endl;
    std::cout << "  Press ENTER to retry...";
    std::cin.get();
    showAddTransactionScreen();
    return;
  }

  // Optional note
  std::cout << "  Note (optional): ";
  std::string note = getInput();
  if (!note.empty()) {
    description += " - " + note;
  }

  std::cout << std::endl;

  // Add transaction and show result
  if (TransactionManager::addTransaction(type, amount, description)) {
    std::cout << std::endl;
    if (type == "income") {
      drawSuccessBox("Income added successfully!");
    } else {
      drawSuccessBox("Expense added successfully!");
    }
  } else {
    drawStatusMessage("Failed to add transaction.", "error");
  }

  // Navigation footer
  drawNavFooter();

  drawPrompt("Press ENTER to continue or 'b' to go back");
  std::string input = getInput();
  if (handleNavigation(input)) return;
  showMainMenu();
}
