#pragma once

#include <iomanip>
#include <iostream>
#include <vector>

#include "../modules/AI.h"
#include "../modules/Transaction.h"
#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showViewTransactionsScreen() {
  clearScreen();

  // Beautiful header
  drawScreenHeader("AI Expense - Transaction History", true);
  std::cout << std::endl;

  std::vector<Transaction> transactions =
      TransactionManager::getAllTransactions();

  if (transactions.empty()) {
    drawInfoBox("No transactions found yet!",
                "Add your first transaction to get started.");
    
    drawNavFooter();
    drawPrompt("Enter command");
    std::string input = getInput();
    if (handleNavigation(input)) return;
    showMainMenu();
    return;
  }

  // Transaction list with beautiful formatting
  drawSectionTitle("Recent Transactions", "📋");
  std::cout << std::endl;

  // Table header
  std::cout << "  ┌──────┬─────────────┬──────────────┬────────────────────────────────┐" << std::endl;
  std::cout << "  │  ID  │    Date     │    Amount    │          Description           │" << std::endl;
  std::cout << "  ├──────┼─────────────┼──────────────┼────────────────────────────────┤" << std::endl;

  for (int i = static_cast<int>(transactions.size()) - 1; i >= 0; i--) {
    const Transaction &t = transactions[i];

    std::cout << "  │ ";
    
    // ID
    if (t.getType() == "income") {
      setColor(10);
    } else {
      setColor(12);
    }
    std::cout << std::setw(4) << t.getId();
    resetColor();
    std::cout << " │ ";

    // Date
    std::cout << std::setw(11) << t.getDate() << " │ ";

    // Amount with color
    if (t.getType() == "income") {
      setColor(10);
      std::cout << "+$" << std::setw(10) << std::fixed << std::setprecision(2) << t.getAmount();
    } else {
      setColor(12);
      std::cout << "-$" << std::setw(10) << std::fixed << std::setprecision(2) << t.getAmount();
    }
    resetColor();
    std::cout << " │ ";

    // Description (truncate if too long)
    std::string desc = t.getDescription();
    if (desc.length() > 30) {
      desc = desc.substr(0, 27) + "...";
    }
    std::cout << std::left << std::setw(30) << desc << std::right << " │" << std::endl;
  }

  std::cout << "  └──────┴─────────────┴──────────────┴────────────────────────────────┘" << std::endl;
  std::cout << std::endl;

  // Summary section
  double totalIncome = TransactionManager::getTotalIncome();
  double totalExpenses = TransactionManager::getTotalExpenses();
  double balance = TransactionManager::getBalance();

  drawSectionTitle("Summary", "📊");
  
  std::cout << "  │ Total Income:   ";
  setColor(10);
  std::cout << "+$" << std::fixed << std::setprecision(2) << totalIncome << std::endl;
  resetColor();

  std::cout << "  │ Total Expenses: ";
  setColor(12);
  std::cout << "-$" << std::fixed << std::setprecision(2) << totalExpenses << std::endl;
  resetColor();

  std::cout << "  │" << std::endl;
  std::cout << "  │ Net Balance:    ";
  if (balance >= 0) {
    setColor(10);
    std::cout << "+$" << std::fixed << std::setprecision(2) << balance << std::endl;
  } else {
    setColor(12);
    std::cout << "-$" << std::fixed << std::setprecision(2) << (-balance) << std::endl;
  }
  resetColor();
  std::cout << std::endl;

  // AI Analysis section
  drawInfoBox("Ask AI about your finances (or press ENTER to go back)");

  // Prepare AI Context
  json conversation_history = json::array();
  conversation_history.push_back(
      {{"role", "system"},
       {"content", "You are a helpful financial advisor. You have access to "
                   "the user's transaction history provided below. Use this "
                   "data to answer questions and give advice. Keep responses concise."}});

  // Serialize transactions to JSON for context
  json trans_json = json::array();
  for (const auto &t : transactions) {
    trans_json.push_back(t.toJson());
  }
  conversation_history.push_back(
      {{"role", "system"},
       {"content", "Transaction History: " + trans_json.dump()}});

  // Chat Loop
  while (true) {
    std::cout << std::endl;
    setColor(11);
    std::cout << "  You: ";
    resetColor();

    std::string input = getInput();

    // Handle navigation
    if (input.empty() || input == "b" || input == "B" || input == "back") {
      break;
    }
    if (input == "m" || input == "M" || input == "menu") {
      break;
    }
    if (handleNavigation(input)) return;

    setColor(8);
    std::cout << "  ⏳ AI is analyzing your transactions..." << std::endl;
    resetColor();

    std::string response = AIModule::chat(input, conversation_history);

    std::cout << std::endl;
    setColor(10);
    std::cout << "  🤖 AI: ";
    resetColor();
    std::cout << response << std::endl;
  }

  showMainMenu();
}
