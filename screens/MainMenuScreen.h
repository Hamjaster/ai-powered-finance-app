#pragma once

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <sstream>

#include "../modules/TransactionManager.h"
#include "../modules/Transaction.h"
#include "../modules/BudgetManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

// Forward declaration
void showBudgetScreen();

inline void showMainMenu() {
  while (true) {
    clearScreen();

    // Header with Unicode border
    drawHeader("AI Expense Manager - Dashboard", "[q]uit");
    std::cout << std::endl;

    // Get transaction data
    std::vector<Transaction> transactions = TransactionManager::getAllTransactions();
    double balance = TransactionManager::getBalance();
    double totalExpenses = TransactionManager::getTotalExpenses();
    double totalIncome = TransactionManager::getTotalIncome();
    
    // Get current month/year
    std::string currentPeriod = getCurrentMonthYear();

    // ═══════════════════════════════════════════════════════════════════════
    // FINANCIAL SUMMARY
    // ═══════════════════════════════════════════════════════════════════════
    // FINANCIAL SUMMARY - Using simpler formatting without complex emoji width calc
    // ═══════════════════════════════════════════════════════════════════════
    
    // Build the financial summary as separate lines
    std::cout << "  ";
    setColor(COLOR_YELLOW);
    std::cout << currentPeriod;
    resetColor();
    std::cout << std::endl;
    std::cout << std::endl;
    
    // Summary line 1: Balance and Income
    std::cout << "  💰 Balance: ";
    if (balance >= 0) setColor(COLOR_GREEN);
    else setColor(COLOR_RED);
    std::cout << formatCurrency(balance);
    resetColor();
    
    std::cout << "          📈 Income: ";
    setColor(COLOR_GREEN);
    std::cout << formatCurrency(totalIncome);
    resetColor();
    std::cout << std::endl;
    
    // Summary line 2: Expenses and Transaction count
    std::cout << "  📉 Expenses: ";
    setColor(COLOR_RED);
    std::cout << formatCurrency(totalExpenses);
    resetColor();
    
    std::cout << "         📋 Transactions: ";
    setColor(COLOR_CYAN);
    std::cout << transactions.size();
    resetColor();
    std::cout << std::endl;

    // ═══════════════════════════════════════════════════════════════════════
    // BUDGET STATUS
    // ═══════════════════════════════════════════════════════════════════════
    
    auto budgets = BudgetManager::getAllBudgets();
    auto alerts = BudgetManager::getAlerts();
    
    if (!budgets.empty()) {
      std::cout << std::endl;
      std::cout << "  ";
      setColor(COLOR_YELLOW);
      std::cout << "Budget Status";
      resetColor();
      std::cout << std::endl;
      std::cout << "  ";
      for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
      std::cout << std::endl;
      
      // Show top 3-4 budgets with progress bars
      int shown = 0;
      for (const auto& budget : budgets) {
        if (shown >= 4) break;
        
        std::string catName = budget.category;
        if (catName.length() > 15) catName = catName.substr(0, 15);
        
        std::cout << "  " << catName;
        for (size_t i = catName.length(); i < 16; i++) std::cout << " ";
        
        drawColoredProgressBar(budget.getPercentUsed(), 25);
        
        std::ostringstream statStr;
        statStr << " $" << std::fixed << std::setprecision(0) << budget.spent 
                << " / $" << budget.limit;
        std::cout << statStr.str();
        
        if (budget.isOverBudget()) {
          setColor(COLOR_RED);
          std::cout << " OVER!";
          resetColor();
        } else if (budget.isWarning()) {
          setColor(COLOR_YELLOW);
          std::cout << " Warning";
          resetColor();
        }
        
        std::cout << std::endl;
        shown++;
      }
    }

    // ═══════════════════════════════════════════════════════════════════════
    // ALERTS
    // ═══════════════════════════════════════════════════════════════════════
    
    if (!alerts.empty()) {
      std::cout << std::endl;
      setColor(COLOR_YELLOW);
      std::cout << "  ⚠ Alerts:" << std::endl;
      resetColor();
      for (const auto& alert : alerts) {
        if (alert.find("OVER") != std::string::npos) {
          std::cout << "  ";
          setColor(COLOR_RED);
          std::cout << "• " << alert << std::endl;
          resetColor();
        } else {
          std::cout << "  ";
          setColor(COLOR_YELLOW);
          std::cout << "• " << alert << std::endl;
          resetColor();
        }
      }
    }

    // ═══════════════════════════════════════════════════════════════════════
    // RECENT TRANSACTIONS
    // ═══════════════════════════════════════════════════════════════════════
    
    std::cout << std::endl;
    std::cout << "  ";
    setColor(COLOR_YELLOW);
    std::cout << "Recent Transactions";
    resetColor();
    std::cout << std::endl;
    std::cout << "  ";
    for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
    std::cout << std::endl;
    
    if (transactions.empty()) {
      setColor(COLOR_GRAY);
      std::cout << "  No transactions yet. Add one with [t] or use quick add!" << std::endl;
      resetColor();
    } else {
      // Show last 5 transactions
      int count = 0;
      for (int i = static_cast<int>(transactions.size()) - 1; i >= 0 && count < 5; i--, count++) {
        const auto& t = transactions[i];
        std::string desc = t.getDescription();
        if (desc.length() > 35) desc = desc.substr(0, 32) + "...";
        
        std::cout << "  " << t.getDate() << "  ";
        
        if (t.getType() == "income") {
          setColor(COLOR_GREEN);
          std::cout << "+";
        } else {
          setColor(COLOR_RED);
          std::cout << "-";
        }
        
        std::ostringstream amt;
        amt << std::fixed << std::setprecision(2) << t.getAmount();
        std::cout << "$" << amt.str();
        resetColor();
        
        // Pad amount
        for (size_t j = amt.str().length(); j < 10; j++) std::cout << " ";
        
        std::cout << " " << desc << std::endl;
      }
    }

    // ═══════════════════════════════════════════════════════════════════════
    // QUICK ADD HINT
    // ═══════════════════════════════════════════════════════════════════════
    
    std::cout << std::endl;
    std::cout << "  ";
    for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
    std::cout << std::endl;
    drawQuickAddHint();

    // ═══════════════════════════════════════════════════════════════════════
    // COMMAND MENU
    // ═══════════════════════════════════════════════════════════════════════
    
    std::cout << std::endl;
    std::cout << "  ";
    setColor(COLOR_CYAN);
    std::cout << "[t]";
    resetColor();
    std::cout << " Add  ";
    
    setColor(COLOR_CYAN);
    std::cout << "[v]";
    resetColor();
    std::cout << " View  ";
    
    setColor(COLOR_CYAN);
    std::cout << "[g]";
    resetColor();
    std::cout << " Graphs  ";
    
    setColor(COLOR_CYAN);
    std::cout << "[b]";
    resetColor();
    std::cout << " Budgets  ";
    
    setColor(COLOR_CYAN);
    std::cout << "[/]";
    resetColor();
    std::cout << " Search  ";
    
    setColor(COLOR_CYAN);
    std::cout << "[e]";
    resetColor();
    std::cout << " Export  ";
    
    setColor(COLOR_CYAN);
    std::cout << "[a]";
    resetColor();
    std::cout << " AI Chat  ";
    
    setColor(COLOR_CYAN);
    std::cout << "[q]";
    resetColor();
    std::cout << " Quit" << std::endl;
    
    drawPrompt("Command or Quick Add");
    std::string command = getInput();

    // Check for quick add format
    if (!command.empty() && (command[0] == '+' || command[0] == '-')) {
      QuickAddResult qa = parseQuickAdd(command);
      if (qa.valid) {
        TransactionManager::addTransaction(qa.type, qa.amount, qa.description);
        std::cout << std::endl;
        drawSuccessBox("Added: " + qa.type + " $" + std::to_string((int)qa.amount) + " - " + qa.description);
        std::cout << "\n  Press any key to continue...";
        _getch();
        continue;
      }
    }

    if (command == "t" || command == "T" || command == "add") {
      showAddTransactionScreen();
    } else if (command == "v" || command == "V" || command == "view" || 
               command == "r" || command == "R") {
      showViewTransactionsScreen();
    } else if (command == "q" || command == "Q" || command == "quit") {
      handleNavigation("q");
    } else if (command == "a" || command == "A" || command == "ai" || 
               command == "c" || command == "C" || command == "chat") {
      showAIScreen();
    } else if (command == "g" || command == "G" || command == "graphs") {
      showGraphsScreen();
    } else if (command == "e" || command == "E" || command == "export") {
      showExportScreen();
    } else if (command == "/" || command == "s" || command == "S" || command == "search") {
      showSearchScreen();
    } else if (command == "b" || command == "B" || command == "budget") {
      showBudgetScreen();
    } else if (!command.empty()) {
      std::cout << std::endl;
      drawStatusMessage("Unknown command: " + command, "error");
      std::cout << "  Press any key to continue...";
      _getch();
    }
  }
}
