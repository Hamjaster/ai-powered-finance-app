#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../modules/Transaction.h"
#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

// Helper to convert string to lowercase
inline std::string toLower(const std::string &str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

// Helper to categorize a transaction
inline std::string categorizeTransaction(const std::string &description) {
  std::string desc = toLower(description);

  if (desc.find("food") != std::string::npos ||
      desc.find("restaurant") != std::string::npos ||
      desc.find("grocery") != std::string::npos ||
      desc.find("cafe") != std::string::npos ||
      desc.find("dining") != std::string::npos ||
      desc.find("meal") != std::string::npos ||
      desc.find("donut") != std::string::npos ||
      desc.find("pizza") != std::string::npos ||
      desc.find("coffee") != std::string::npos) {
    return "Food";
  } else if (desc.find("transport") != std::string::npos ||
             desc.find("uber") != std::string::npos ||
             desc.find("taxi") != std::string::npos ||
             desc.find("gas") != std::string::npos ||
             desc.find("fuel") != std::string::npos ||
             desc.find("bus") != std::string::npos ||
             desc.find("train") != std::string::npos ||
             desc.find("metro") != std::string::npos ||
             desc.find("car") != std::string::npos) {
    return "Transport";
  } else if (desc.find("rent") != std::string::npos ||
             desc.find("housing") != std::string::npos ||
             desc.find("mortgage") != std::string::npos ||
             desc.find("apartment") != std::string::npos) {
    return "Housing";
  } else if (desc.find("utility") != std::string::npos ||
             desc.find("electric") != std::string::npos ||
             desc.find("water") != std::string::npos ||
             desc.find("internet") != std::string::npos ||
             desc.find("phone") != std::string::npos ||
             desc.find("power") != std::string::npos ||
             desc.find("bill") != std::string::npos) {
    return "Utilities";
  } else if (desc.find("shop") != std::string::npos ||
             desc.find("buy") != std::string::npos ||
             desc.find("store") != std::string::npos ||
             desc.find("amazon") != std::string::npos ||
             desc.find("purchase") != std::string::npos) {
    return "Shopping";
  } else if (desc.find("entertainment") != std::string::npos ||
             desc.find("movie") != std::string::npos ||
             desc.find("game") != std::string::npos ||
             desc.find("netflix") != std::string::npos ||
             desc.find("spotify") != std::string::npos ||
             desc.find("subscription") != std::string::npos) {
    return "Entertainment";
  } else if (desc.find("health") != std::string::npos ||
             desc.find("doctor") != std::string::npos ||
             desc.find("medicine") != std::string::npos ||
             desc.find("pharmacy") != std::string::npos ||
             desc.find("hospital") != std::string::npos) {
    return "Health";
  } else if (desc.find("salary") != std::string::npos ||
             desc.find("job") != std::string::npos ||
             desc.find("income") != std::string::npos ||
             desc.find("paycheck") != std::string::npos ||
             desc.find("wage") != std::string::npos) {
    return "Salary";
  }
  return "Other";
}

// Display filtered transactions
inline void displayFilteredTransactions(const std::vector<Transaction> &transactions) {
  if (transactions.empty()) {
    std::cout << std::endl;
    std::cout << "  ╭────────────────────────────────────────────────────────────────────╮" << std::endl;
    std::cout << "  │              🔍 No transactions match your criteria                │" << std::endl;
    std::cout << "  ╰────────────────────────────────────────────────────────────────────╯" << std::endl;
    return;
  }

  std::cout << std::endl;
  drawSectionTitle("Search Results (" + std::to_string(transactions.size()) + " found)", "🔍");
  std::cout << std::endl;

  // Calculate totals for filtered results
  double totalIncome = 0, totalExpenses = 0;
  for (const auto &t : transactions) {
    if (t.getType() == "income") totalIncome += t.getAmount();
    else totalExpenses += t.getAmount();
  }

  // Table header
  std::cout << "  ┌──────┬─────────────┬──────────────┬──────────────┬────────────────────┐" << std::endl;
  std::cout << "  │  ID  │    Date     │   Category   │    Amount    │    Description     │" << std::endl;
  std::cout << "  ├──────┼─────────────┼──────────────┼──────────────┼────────────────────┤" << std::endl;

  for (const auto &t : transactions) {
    std::string category = categorizeTransaction(t.getDescription());
    
    std::cout << "  │ ";
    
    // ID with color
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

    // Category
    std::cout << std::setw(12) << category << " │ ";

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
    if (desc.length() > 18) {
      desc = desc.substr(0, 15) + "...";
    }
    std::cout << std::left << std::setw(18) << desc << std::right << " │" << std::endl;
  }

  std::cout << "  └──────┴─────────────┴──────────────┴──────────────┴────────────────────┘" << std::endl;

  // Summary of filtered results
  std::cout << std::endl;
  std::cout << "  📊 Filtered Summary: ";
  setColor(10);
  std::cout << "+$" << std::fixed << std::setprecision(2) << totalIncome;
  resetColor();
  std::cout << " income, ";
  setColor(12);
  std::cout << "-$" << totalExpenses;
  resetColor();
  std::cout << " expenses, ";
  double net = totalIncome - totalExpenses;
  if (net >= 0) {
    setColor(10);
    std::cout << "+$" << net;
  } else {
    setColor(12);
    std::cout << "-$" << (-net);
  }
  resetColor();
  std::cout << " net" << std::endl;
}

inline void showSearchScreen() {
  clearScreen();

  // Header
  drawScreenHeader("AI Expense • Search & Filter", true);
  std::cout << std::endl;

  // Get all transactions
  std::vector<Transaction> allTransactions = TransactionManager::getAllTransactions();

  if (allTransactions.empty()) {
    std::cout << "  ╭────────────────────────────────────────────────────────────────────╮" << std::endl;
    std::cout << "  │              📭 No transactions to search!                         │" << std::endl;
    std::cout << "  │         Add some transactions first.                              │" << std::endl;
    std::cout << "  ╰────────────────────────────────────────────────────────────────────╯" << std::endl;
    
    drawNavFooter();
    drawPrompt("Press ENTER to go back");
    getInput();
    showMainMenu();
    return;
  }

  // Info box
  std::cout << "  ╭────────────────────────────────────────────────────────────────────╮" << std::endl;
  std::cout << "  │  🔍 Search and filter your transactions                            │" << std::endl;
  std::cout << "  │     Use multiple filters to narrow down results.                  │" << std::endl;
  std::cout << "  ╰────────────────────────────────────────────────────────────────────╯" << std::endl;
  std::cout << std::endl;

  drawInfoLine("📊", "Total transactions", std::to_string(allTransactions.size()), 14);
  std::cout << std::endl;

  // Search options
  drawSectionTitle("Search Options", "🔎");
  std::cout << std::endl;

  drawMenuOption("1", "Search by keyword", "🔤");
  drawMenuOption("2", "Filter by type (Income/Expense)", "💰");
  drawMenuOption("3", "Filter by category", "📁");
  drawMenuOption("4", "Filter by amount range", "💵");
  drawMenuOption("5", "Filter by date", "📅");
  drawMenuOption("6", "Advanced search (multiple filters)", "⚙️");
  std::cout << std::endl;
  drawMenuOption("b", "Go Back", "←");

  drawPrompt("Choice");
  std::string choice = getInput();

  // Handle navigation
  if (choice == "b" || choice == "B" || choice == "back") {
    showMainMenu();
    return;
  }
  if (handleNavigation(choice)) return;

  std::vector<Transaction> results;

  if (choice == "1") {
    // Search by keyword
    clearScreen();
    drawScreenHeader("AI Expense • Search by Keyword", true);
    std::cout << std::endl;

    std::cout << "  Enter search keyword (searches in description):" << std::endl;
    drawPrompt("Keyword");
    std::string keyword = getInput();

    if (keyword.empty()) {
      results = allTransactions;
    } else {
      std::string keywordLower = toLower(keyword);
      for (const auto &t : allTransactions) {
        if (toLower(t.getDescription()).find(keywordLower) != std::string::npos) {
          results.push_back(t);
        }
      }
    }

    displayFilteredTransactions(results);

  } else if (choice == "2") {
    // Filter by type
    clearScreen();
    drawScreenHeader("AI Expense • Filter by Type", true);
    std::cout << std::endl;

    drawMenuOption("1", "Show only Income", "💰");
    drawMenuOption("2", "Show only Expenses", "💸");

    drawPrompt("Choice");
    std::string typeChoice = getInput();

    std::string filterType = (typeChoice == "1") ? "income" : "expense";
    
    for (const auto &t : allTransactions) {
      if (t.getType() == filterType) {
        results.push_back(t);
      }
    }

    displayFilteredTransactions(results);

  } else if (choice == "3") {
    // Filter by category
    clearScreen();
    drawScreenHeader("AI Expense • Filter by Category", true);
    std::cout << std::endl;

    drawMenuOption("1", "Food", "🍔");
    drawMenuOption("2", "Transport", "🚗");
    drawMenuOption("3", "Housing", "🏠");
    drawMenuOption("4", "Utilities", "💡");
    drawMenuOption("5", "Shopping", "🛒");
    drawMenuOption("6", "Entertainment", "🎮");
    drawMenuOption("7", "Health", "🏥");
    drawMenuOption("8", "Salary/Income", "💼");
    drawMenuOption("9", "Other", "📦");

    drawPrompt("Choice");
    std::string catChoice = getInput();

    std::string filterCategory;
    switch (catChoice[0]) {
      case '1': filterCategory = "Food"; break;
      case '2': filterCategory = "Transport"; break;
      case '3': filterCategory = "Housing"; break;
      case '4': filterCategory = "Utilities"; break;
      case '5': filterCategory = "Shopping"; break;
      case '6': filterCategory = "Entertainment"; break;
      case '7': filterCategory = "Health"; break;
      case '8': filterCategory = "Salary"; break;
      default: filterCategory = "Other"; break;
    }

    for (const auto &t : allTransactions) {
      if (categorizeTransaction(t.getDescription()) == filterCategory) {
        results.push_back(t);
      }
    }

    displayFilteredTransactions(results);

  } else if (choice == "4") {
    // Filter by amount range
    clearScreen();
    drawScreenHeader("AI Expense • Filter by Amount", true);
    std::cout << std::endl;

    std::cout << "  Enter amount range (leave blank for no limit):" << std::endl;
    std::cout << std::endl;

    std::cout << "  Minimum amount: $";
    std::string minStr = getInput();
    double minAmount = minStr.empty() ? 0 : std::stod(minStr);

    std::cout << "  Maximum amount: $";
    std::string maxStr = getInput();
    double maxAmount = maxStr.empty() ? 999999999 : std::stod(maxStr);

    for (const auto &t : allTransactions) {
      if (t.getAmount() >= minAmount && t.getAmount() <= maxAmount) {
        results.push_back(t);
      }
    }

    displayFilteredTransactions(results);

  } else if (choice == "5") {
    // Filter by date
    clearScreen();
    drawScreenHeader("AI Expense • Filter by Date", true);
    std::cout << std::endl;

    std::cout << "  Enter month to filter (e.g., 'Nov' or 'November'):" << std::endl;
    drawPrompt("Month");
    std::string monthFilter = getInput();

    std::string monthLower = toLower(monthFilter);

    for (const auto &t : allTransactions) {
      std::string dateLower = toLower(t.getDate());
      if (dateLower.find(monthLower) != std::string::npos) {
        results.push_back(t);
      }
    }

    displayFilteredTransactions(results);

  } else if (choice == "6") {
    // Advanced search with multiple filters
    clearScreen();
    drawScreenHeader("AI Expense • Advanced Search", true);
    std::cout << std::endl;

    std::cout << "  ╭────────────────────────────────────────────────────────────────────╮" << std::endl;
    std::cout << "  │  ⚙️  Advanced Search - Apply multiple filters                      │" << std::endl;
    std::cout << "  │     Leave any field blank to skip that filter.                   │" << std::endl;
    std::cout << "  ╰────────────────────────────────────────────────────────────────────╯" << std::endl;
    std::cout << std::endl;

    // Keyword filter
    std::cout << "  Keyword (searches description): ";
    std::string keyword = getInput();

    // Type filter
    std::cout << "  Type [i]ncome / [e]xpense / [a]ll: ";
    std::string typeFilter = getInput();

    // Amount range
    std::cout << "  Minimum amount (blank for none): $";
    std::string minStr = getInput();
    double minAmount = minStr.empty() ? 0 : std::stod(minStr);

    std::cout << "  Maximum amount (blank for none): $";
    std::string maxStr = getInput();
    double maxAmount = maxStr.empty() ? 999999999 : std::stod(maxStr);

    // Month filter
    std::cout << "  Month filter (e.g., Nov): ";
    std::string monthFilter = getInput();

    // Apply all filters
    std::string keywordLower = toLower(keyword);
    std::string monthLower = toLower(monthFilter);

    for (const auto &t : allTransactions) {
      bool matches = true;

      // Keyword filter
      if (!keyword.empty()) {
        if (toLower(t.getDescription()).find(keywordLower) == std::string::npos) {
          matches = false;
        }
      }

      // Type filter
      if (!typeFilter.empty() && typeFilter != "a" && typeFilter != "A" && typeFilter != "all") {
        std::string expectedType = (typeFilter == "i" || typeFilter == "I" || typeFilter == "income") 
                                   ? "income" : "expense";
        if (t.getType() != expectedType) {
          matches = false;
        }
      }

      // Amount range filter
      if (t.getAmount() < minAmount || t.getAmount() > maxAmount) {
        matches = false;
      }

      // Month filter
      if (!monthFilter.empty()) {
        if (toLower(t.getDate()).find(monthLower) == std::string::npos) {
          matches = false;
        }
      }

      if (matches) {
        results.push_back(t);
      }
    }

    displayFilteredTransactions(results);

  } else {
    drawStatusMessage("Invalid choice.", "error");
  }

  drawNavFooter();
  drawPrompt("Press ENTER to continue or 'b' to go back");
  std::string input = getInput();
  if (handleNavigation(input)) return;
  showSearchScreen();
}
