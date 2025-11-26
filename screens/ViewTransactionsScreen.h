#pragma once

#include <iomanip>
#include <iostream>
#include <vector>

#include "../modules/AI.h" // Added for AIModule
#include "../modules/Transaction.h"
#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showViewTransactionsScreen() {
  clearScreen();

  drawBoxedTitle("View Transactions");
  std::cout << std::endl;

  std::vector<Transaction> transactions =
      TransactionManager::getAllTransactions();

  if (transactions.empty()) {
    std::cout << "No transactions found." << std::endl;
    std::cout << std::endl;
  } else {
    for (int i = static_cast<int>(transactions.size()) - 1; i >= 0; i--) {
      const Transaction &t = transactions[i];

      if (t.getType() == "income") {
        setColor(10);
      } else {
        setColor(12);
      }

      std::cout << "[" << t.getId() << "] ";
      resetColor();

      std::cout << t.getDate() << " - ";

      if (t.getType() == "income") {
        setColor(10);
        std::cout << "+$";
      } else {
        setColor(12);
        std::cout << "-$";
      }

      std::cout << std::fixed << std::setprecision(2) << t.getAmount();
      resetColor();

      std::cout << " - " << t.getDescription() << std::endl;
    }
    std::cout << std::endl;

    double totalIncome = TransactionManager::getTotalIncome();
    double totalExpenses = TransactionManager::getTotalExpenses();
    double balance = TransactionManager::getBalance();

    drawSeparator();
    std::cout << "  Summary:" << std::endl;
    std::cout << "  Total Income:   ";
    setColor(10);
    std::cout << "$" << std::fixed << std::setprecision(2) << totalIncome
              << std::endl;
    resetColor();

    std::cout << "  Total Expenses: ";
    setColor(12);
    std::cout << "$" << std::fixed << std::setprecision(2) << totalExpenses
              << std::endl;
    resetColor();

    std::cout << "  Net Balance:    ";
    if (balance >= 0) {
      setColor(10);
      std::cout << "$";
    } else {
      setColor(12);
      std::cout << "-$";
      balance = -balance;
    }
    std::cout << std::fixed << std::setprecision(2) << balance << std::endl;
    resetColor();
    std::cout << std::endl;
  }

  // Prepare AI Context
  json conversation_history = json::array();
  conversation_history.push_back(
      {{"role", "system"},
       {"content", "You are a helpful financial advisor. You have access to "
                   "the user's transaction history provided below. Use this "
                   "data to answer questions and give advice."}});

  // Serialize transactions to JSON for context
  json trans_json = json::array();
  for (const auto &t : transactions) {
    trans_json.push_back(t.toJson());
  }
  conversation_history.push_back(
      {{"role", "system"},
       {"content", "Transaction History: " + trans_json.dump()}});

  drawSeparator();
  std::cout << "  Ask AI about your finances below." << std::endl;
  std::cout << "  (Press ENTER to return to menu)" << std::endl;
  drawSeparator();

  // Chat Loop
  while (true) {
    setColor(11); // User Cyan
    std::cout << "\nYou: ";
    resetColor();

    std::string input = getInput();

    if (input.empty()) {
      break; // Exit to main menu
    }

    setColor(14); // Thinking Yellow
    std::cout << "AI is analyzing...";
    resetColor();

    // Call AI
    std::string response = AIModule::chat(input, conversation_history);

    // Clear thinking line
    std::cout << "\r                    \r";

    setColor(10); // AI Green
    std::cout << "AI: " << response << std::endl;
    resetColor();
  }

  showMainMenu();
}
