#pragma once

#include "../modules/BudgetManager.h"
#include "../modules/TransactionManager.h"
#include "ScreenUtils.h"
#include <iomanip>
#include <sstream>

inline void showBudgetScreen() {
  while (true) {
    clearScreen();
    drawScreenHeader("Budget Management", true);
    
    // Get all budgets
    auto budgets = BudgetManager::getAllBudgets();
    
    // Display current month
    std::cout << std::endl;
    std::cout << "  📅 Current Period: ";
    setColor(COLOR_CYAN);
    std::cout << getCurrentMonthYear() << std::endl;
    resetColor();
    
    // Budget overview section
    drawSectionTitle("Category Budgets", "💰");
    
    if (budgets.empty()) {
      std::cout << std::endl;
      setColor(COLOR_GRAY);
      std::cout << "  No budgets set. Add a budget to start tracking!" << std::endl;
      resetColor();
    } else {
      // Table header
      std::cout << std::endl;
      std::cout << "  ┌──────────────────┬────────────┬────────────┬────────────────────────────┐" << std::endl;
      std::cout << "  │ ";
      setColor(COLOR_YELLOW);
      std::cout << "Category";
      resetColor();
      std::cout << "         │ ";
      setColor(COLOR_YELLOW);
      std::cout << "Budget";
      resetColor();
      std::cout << "     │ ";
      setColor(COLOR_YELLOW);
      std::cout << "Spent";
      resetColor();
      std::cout << "      │ ";
      setColor(COLOR_YELLOW);
      std::cout << "Progress";
      resetColor();
      std::cout << "                   │" << std::endl;
      std::cout << "  ├──────────────────┼────────────┼────────────┼────────────────────────────┤" << std::endl;
      
      for (const auto& budget : budgets) {
        // Category name (padded to 16 chars)
        std::string catName = budget.category;
        if (catName.length() > 16) catName = catName.substr(0, 16);
        
        // Format amounts
        std::ostringstream limitStr, spentStr;
        limitStr << std::fixed << std::setprecision(0) << "$" << budget.limit;
        spentStr << std::fixed << std::setprecision(0) << "$" << budget.spent;
        
        std::cout << "  │ " << catName;
        for (size_t i = catName.length(); i < 17; i++) std::cout << " ";
        std::cout << "│ " << limitStr.str();
        for (size_t i = limitStr.str().length(); i < 11; i++) std::cout << " ";
        std::cout << "│ ";
        
        // Spent amount with color
        if (budget.isOverBudget()) {
          setColor(COLOR_RED);
        } else if (budget.isWarning()) {
          setColor(COLOR_YELLOW);
        } else {
          setColor(COLOR_GREEN);
        }
        std::cout << spentStr.str();
        resetColor();
        for (size_t i = spentStr.str().length(); i < 11; i++) std::cout << " ";
        std::cout << "│ ";
        
        // Progress bar (20 chars)
        drawColoredProgressBar(budget.getPercentUsed(), 20);
        
        // Percentage
        std::ostringstream pctStr;
        pctStr << " " << std::fixed << std::setprecision(0) << budget.getPercentUsed() << "%";
        std::cout << pctStr.str();
        for (size_t i = pctStr.str().length(); i < 6; i++) std::cout << " ";
        std::cout << "│" << std::endl;
      }
      
      std::cout << "  └──────────────────┴────────────┴────────────┴────────────────────────────┘" << std::endl;
    }
    
    // Alerts section
    auto alerts = BudgetManager::getAlerts();
    if (!alerts.empty()) {
      drawSectionTitle("Alerts", "⚠️");
      for (const auto& alert : alerts) {
        if (alert.find("OVER") != std::string::npos) {
          drawStatusMessage(alert, "error");
        } else {
          drawStatusMessage(alert, "warning");
        }
      }
    }
    
    // Menu options
    drawSectionTitle("Options", "⚙️");
    drawMenuOption("a", "Add/Edit Budget");
    drawMenuOption("d", "Delete a Budget");
    
    drawNavFooter();
    drawPrompt("Choose an option");
    
    std::string choice = getInput();
    
    if (handleNavigation(choice)) continue;
    if (choice == "b" || choice == "B" || choice == "m" || choice == "M") return;
    
    if (choice == "a" || choice == "A") {
      // Add/Edit budget
      clearScreen();
      drawScreenHeader("Add/Edit Budget", true);
      
      std::cout << std::endl;
      std::cout << "  Available categories:" << std::endl;
      std::cout << "  1. Food" << std::endl;
      std::cout << "  2. Transport" << std::endl;
      std::cout << "  3. Housing" << std::endl;
      std::cout << "  4. Utilities" << std::endl;
      std::cout << "  5. Shopping" << std::endl;
      std::cout << "  6. Entertainment" << std::endl;
      std::cout << "  7. Health" << std::endl;
      std::cout << "  8. Other" << std::endl;
      std::cout << "  9. Custom category" << std::endl;
      
      drawPrompt("Enter category number (1-9)");
      std::string catChoice = getInput();
      
      std::string category;
      switch (catChoice[0]) {
        case '1': category = "Food"; break;
        case '2': category = "Transport"; break;
        case '3': category = "Housing"; break;
        case '4': category = "Utilities"; break;
        case '5': category = "Shopping"; break;
        case '6': category = "Entertainment"; break;
        case '7': category = "Health"; break;
        case '8': category = "Other"; break;
        case '9':
          drawPrompt("Enter custom category name");
          category = getInput();
          break;
        default:
          drawStatusMessage("Invalid choice", "error");
          std::cout << "\n  Press any key to continue...";
          _getch();
          continue;
      }
      
      if (category.empty()) continue;
      
      drawPrompt("Enter monthly budget limit ($)");
      double limit = getDoubleInput();
      
      if (limit <= 0) {
        drawStatusMessage("Invalid amount", "error");
        std::cout << "\n  Press any key to continue...";
        _getch();
        continue;
      }
      
      BudgetManager::setBudget(category, limit);
      
      std::cout << std::endl;
      drawSuccessBox("Budget set: " + category + " = $" + std::to_string((int)limit));
      std::cout << "\n  Press any key to continue...";
      _getch();
      
    } else if (choice == "d" || choice == "D") {
      // Delete a budget
      clearScreen();
      drawScreenHeader("Delete Budget", true);
      
      if (budgets.empty()) {
        std::cout << std::endl;
        drawStatusMessage("No budgets to delete.", "info");
        std::cout << "\n  Press any key to continue...";
        _getch();
        continue;
      }
      
      std::cout << std::endl;
      std::cout << "  Select budget to delete:" << std::endl;
      for (size_t i = 0; i < budgets.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << budgets[i].category << std::endl;
      }
      
      drawPrompt("Enter number to delete (or 'c' to cancel)");
      std::string delChoice = getInput();
      
      if (delChoice == "c" || delChoice == "C") continue;
      
      try {
        int idx = std::stoi(delChoice) - 1;
        if (idx >= 0 && idx < static_cast<int>(budgets.size())) {
          std::string catToDelete = budgets[idx].category;
          BudgetManager::deleteBudget(catToDelete);
          drawSuccessBox("Deleted budget for: " + catToDelete);
        } else {
          drawStatusMessage("Invalid selection", "error");
        }
      } catch (...) {
        drawStatusMessage("Invalid input", "error");
      }
      std::cout << "\n  Press any key to continue...";
      _getch();
    }
  }
}
