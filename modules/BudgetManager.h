#pragma once

#include "../include/nlohmann/json.hpp"
#include "FileHandler.h"
#include "Transaction.h"
#include "TransactionManager.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;

struct Budget {
  std::string category;
  double limit;
  double spent;

  double getPercentUsed() const {
    if (limit <= 0) return 0;
    return (spent / limit) * 100.0;
  }

  double getRemaining() const {
    return limit - spent;
  }

  bool isOverBudget() const {
    return spent > limit;
  }

  bool isWarning() const {
    return getPercentUsed() >= 80.0 && !isOverBudget();
  }
};

class BudgetManager {
private:
  static std::string getBudgetFilePath() {
    return "data/budgets.json";
  }

public:
  // Default budgets
  static std::map<std::string, double> getDefaultBudgets() {
    return {
      {"Food", 400.0},
      {"Transport", 200.0},
      {"Housing", 1500.0},
      {"Utilities", 150.0},
      {"Shopping", 300.0},
      {"Entertainment", 200.0},
      {"Health", 100.0},
      {"Other", 500.0}
    };
  }

  // Load budgets from file
  static std::map<std::string, double> loadBudgets() {
    std::ifstream file(getBudgetFilePath());
    if (!file.is_open()) {
      return getDefaultBudgets();
    }

    try {
      json j;
      file >> j;
      file.close();

      std::map<std::string, double> budgets;
      for (auto& [key, value] : j["budgets"].items()) {
        budgets[key] = value.get<double>();
      }
      return budgets;
    } catch (...) {
      return getDefaultBudgets();
    }
  }

  // Save budgets to file
  static bool saveBudgets(const std::map<std::string, double>& budgets) {
    json j;
    j["budgets"] = budgets;

    std::ofstream file(getBudgetFilePath());
    if (!file.is_open()) {
      return false;
    }

    file << j.dump(2);
    file.close();
    return true;
  }

  // Set budget for a category
  static bool setBudget(const std::string& category, double limit) {
    if (limit < 0) return false;
    
    auto budgets = loadBudgets();
    budgets[category] = limit;
    return saveBudgets(budgets);
  }

  // Categorize transaction
  static std::string categorize(const std::string& description) {
    std::string desc = description;
    std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);

    if (desc.find("food") != std::string::npos ||
        desc.find("restaurant") != std::string::npos ||
        desc.find("grocery") != std::string::npos ||
        desc.find("cafe") != std::string::npos ||
        desc.find("dining") != std::string::npos ||
        desc.find("meal") != std::string::npos ||
        desc.find("lunch") != std::string::npos ||
        desc.find("dinner") != std::string::npos ||
        desc.find("breakfast") != std::string::npos ||
        desc.find("coffee") != std::string::npos ||
        desc.find("pizza") != std::string::npos ||
        desc.find("donut") != std::string::npos) {
      return "Food";
    }
    
    if (desc.find("transport") != std::string::npos ||
        desc.find("uber") != std::string::npos ||
        desc.find("lyft") != std::string::npos ||
        desc.find("taxi") != std::string::npos ||
        desc.find("gas") != std::string::npos ||
        desc.find("fuel") != std::string::npos ||
        desc.find("bus") != std::string::npos ||
        desc.find("train") != std::string::npos ||
        desc.find("metro") != std::string::npos ||
        desc.find("car") != std::string::npos) {
      return "Transport";
    }
    
    if (desc.find("rent") != std::string::npos ||
        desc.find("housing") != std::string::npos ||
        desc.find("mortgage") != std::string::npos ||
        desc.find("apartment") != std::string::npos) {
      return "Housing";
    }
    
    if (desc.find("utility") != std::string::npos ||
        desc.find("electric") != std::string::npos ||
        desc.find("water") != std::string::npos ||
        desc.find("internet") != std::string::npos ||
        desc.find("phone") != std::string::npos ||
        desc.find("power") != std::string::npos ||
        desc.find("bill") != std::string::npos) {
      return "Utilities";
    }
    
    if (desc.find("shop") != std::string::npos ||
        desc.find("amazon") != std::string::npos ||
        desc.find("store") != std::string::npos ||
        desc.find("mall") != std::string::npos ||
        desc.find("clothes") != std::string::npos ||
        desc.find("buy") != std::string::npos) {
      return "Shopping";
    }
    
    if (desc.find("entertainment") != std::string::npos ||
        desc.find("movie") != std::string::npos ||
        desc.find("game") != std::string::npos ||
        desc.find("netflix") != std::string::npos ||
        desc.find("spotify") != std::string::npos ||
        desc.find("subscription") != std::string::npos ||
        desc.find("concert") != std::string::npos) {
      return "Entertainment";
    }
    
    if (desc.find("health") != std::string::npos ||
        desc.find("doctor") != std::string::npos ||
        desc.find("medicine") != std::string::npos ||
        desc.find("pharmacy") != std::string::npos ||
        desc.find("hospital") != std::string::npos ||
        desc.find("gym") != std::string::npos) {
      return "Health";
    }

    return "Other";
  }

  // Get all budgets with current spending
  static std::vector<Budget> getAllBudgets() {
    auto budgetLimits = loadBudgets();
    auto transactions = TransactionManager::getAllTransactions();

    // Calculate spending per category
    std::map<std::string, double> categorySpent;
    for (const auto& t : transactions) {
      if (t.getType() == "expense") {
        std::string category = categorize(t.getDescription());
        categorySpent[category] += t.getAmount();
      }
    }

    // Build budget list
    std::vector<Budget> result;
    for (const auto& [category, limit] : budgetLimits) {
      Budget b;
      b.category = category;
      b.limit = limit;
      b.spent = categorySpent[category];
      result.push_back(b);
    }

    // Sort by percent used (highest first)
    std::sort(result.begin(), result.end(), [](const Budget& a, const Budget& b) {
      return a.getPercentUsed() > b.getPercentUsed();
    });

    return result;
  }

  // Get budgets that are over limit or in warning (returns strings)
  static std::vector<std::string> getAlerts() {
    std::vector<std::string> alerts;
    for (const auto& b : getAllBudgets()) {
      if (b.isOverBudget()) {
        std::ostringstream oss;
        oss << b.category << " is OVER budget! ($" << std::fixed << std::setprecision(0) 
            << b.spent << " / $" << b.limit << ")";
        alerts.push_back(oss.str());
      } else if (b.isWarning()) {
        std::ostringstream oss;
        oss << b.category << " is at " << std::fixed << std::setprecision(0) 
            << b.getPercentUsed() << "% of budget";
        alerts.push_back(oss.str());
      }
    }
    return alerts;
  }

  // Delete a budget category
  static bool deleteBudget(const std::string& category) {
    auto budgets = loadBudgets();
    if (budgets.find(category) != budgets.end()) {
      budgets.erase(category);
      return saveBudgets(budgets);
    }
    return false;
  }

  // Reset all spending (note: spending is calculated from transactions, 
  // so this would need to delete transactions or use a different approach)
  static void resetAllSpending() {
    // In the current design, spending is calculated from transactions
    // This is a placeholder - in a real app you might want to reset a month marker
  }

  // Get total budget limit
  static double getTotalBudget() {
    double total = 0;
    for (const auto& [cat, limit] : loadBudgets()) {
      total += limit;
    }
    return total;
  }
};
