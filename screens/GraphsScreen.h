#pragma once

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../modules/Transaction.h"
#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

// Helper to get month name from index
inline std::string getMonthName(int month) {
  const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  if (month >= 0 && month < 12)
    return months[month];
  return "???";
}

// Parse date string "15 Nov, 25" to month index and year
inline bool parseDateToMonthYear(const std::string &dateStr, int &month,
                                  int &year) {
  const std::map<std::string, int> monthMap = {
      {"Jan", 0}, {"Feb", 1}, {"Mar", 2},  {"Apr", 3},
      {"May", 4}, {"Jun", 5}, {"Jul", 6},  {"Aug", 7},
      {"Sep", 8}, {"Oct", 9}, {"Nov", 10}, {"Dec", 11}};

  // Format: "15 Nov, 25"
  std::istringstream iss(dateStr);
  int day;
  std::string monthStr;
  char comma;
  int yr;

  if (iss >> day >> monthStr) {
    // Remove comma if present
    if (!monthStr.empty() && monthStr.back() == ',') {
      monthStr.pop_back();
    }

    auto it = monthMap.find(monthStr);
    if (it != monthMap.end()) {
      month = it->second;

      // Try to read year
      if (iss >> comma >> yr || iss >> yr) {
        year = 2000 + yr; // Assuming 2000s
        return true;
      } else {
        // Use current year as fallback
        time_t now = time(0);
        tm *timeinfo = localtime(&now);
        year = 1900 + timeinfo->tm_year;
        return true;
      }
    }
  }
  return false;
}

// Draw a horizontal bar chart
inline void drawHorizontalBar(const std::string &label, double value,
                               double maxValue, int maxBarWidth,
                               bool isExpense = true) {
  int barWidth = 0;
  if (maxValue > 0) {
    barWidth = static_cast<int>((value / maxValue) * maxBarWidth);
  }
  if (barWidth < 0)
    barWidth = 0;
  if (barWidth > maxBarWidth)
    barWidth = maxBarWidth;

  // Label (fixed width)
  std::cout << "  " << std::left << std::setw(12) << label << " ";

  // Bar
  if (isExpense) {
    setColor(12); // Red for expenses
  } else {
    setColor(10); // Green for income
  }

  std::cout << "[";
  for (int i = 0; i < barWidth; i++) {
    std::cout << "█";
  }
  for (int i = barWidth; i < maxBarWidth; i++) {
    std::cout << " ";
  }
  std::cout << "] ";

  resetColor();

  // Value
  std::cout << "$" << std::fixed << std::setprecision(2) << value << std::endl;
}

// Draw a vertical bar for the monthly chart
inline void drawVerticalBars(const std::vector<std::string> &labels,
                              const std::vector<double> &expenses,
                              const std::vector<double> &incomes, int height) {
  double maxVal = 0;
  for (size_t i = 0; i < expenses.size(); i++) {
    if (expenses[i] > maxVal)
      maxVal = expenses[i];
    if (i < incomes.size() && incomes[i] > maxVal)
      maxVal = incomes[i];
  }

  if (maxVal == 0)
    maxVal = 100; // Prevent division by zero

  // Draw from top to bottom
  for (int row = height; row >= 1; row--) {
    double threshold = (static_cast<double>(row) / height) * maxVal;

    // Y-axis label
    if (row == height) {
      std::cout << "  " << std::setw(8) << std::right << std::fixed
                << std::setprecision(0) << maxVal << " |";
    } else if (row == height / 2) {
      std::cout << "  " << std::setw(8) << std::right << std::fixed
                << std::setprecision(0) << (maxVal / 2) << " |";
    } else if (row == 1) {
      std::cout << "  " << std::setw(8) << std::right << "0"
                << " |";
    } else {
      std::cout << "           |";
    }

    // Draw bars for each month
    for (size_t i = 0; i < labels.size(); i++) {
      std::cout << " ";

      // Expense bar
      if (expenses[i] >= threshold) {
        setColor(12); // Red
        std::cout << "██";
        resetColor();
      } else {
        std::cout << "  ";
      }

      // Income bar
      if (i < incomes.size() && incomes[i] >= threshold) {
        setColor(10); // Green
        std::cout << "██";
        resetColor();
      } else {
        std::cout << "  ";
      }

      std::cout << " ";
    }
    std::cout << std::endl;
  }

  // X-axis
  std::cout << "           +";
  for (size_t i = 0; i < labels.size(); i++) {
    std::cout << "-------";
  }
  std::cout << std::endl;

  // Labels
  std::cout << "            ";
  for (size_t i = 0; i < labels.size(); i++) {
    std::cout << " " << std::setw(5) << labels[i] << " ";
  }
  std::cout << std::endl;

  // Legend
  std::cout << std::endl;
  std::cout << "            ";
  setColor(12);
  std::cout << "██";
  resetColor();
  std::cout << " Expenses   ";
  setColor(10);
  std::cout << "██";
  resetColor();
  std::cout << " Income" << std::endl;
}

// Draw a pie chart representation using ASCII
inline void drawPieChartASCII(const std::map<std::string, double> &data,
                               double total) {
  if (total <= 0) {
    std::cout << "  No data to display." << std::endl;
    return;
  }

  // Colors for categories
  std::map<std::string, int> categoryColors = {{"Food", 14},      // Yellow
                                                {"Transport", 11}, // Cyan
                                                {"Rent", 13},      // Magenta
                                                {"Utilities", 10}, // Green
                                                {"Other", 8}};     // Gray

  // Sort by value descending
  std::vector<std::pair<std::string, double>> sorted(data.begin(), data.end());
  std::sort(sorted.begin(), sorted.end(),
            [](const auto &a, const auto &b) { return a.second > b.second; });

  int barWidth = 40;

  std::cout << std::endl;
  for (const auto &item : sorted) {
    double percentage = (item.second / total) * 100;
    int filled = static_cast<int>((item.second / total) * barWidth);

    std::cout << "  " << std::left << std::setw(12) << item.first;

    int color = 7;
    if (categoryColors.find(item.first) != categoryColors.end()) {
      color = categoryColors[item.first];
    }

    setColor(color);
    for (int i = 0; i < filled; i++) {
      std::cout << "█";
    }
    resetColor();

    for (int i = filled; i < barWidth; i++) {
      std::cout << "░";
    }

    std::cout << " " << std::fixed << std::setprecision(1) << percentage << "% ($"
              << std::setprecision(2) << item.second << ")" << std::endl;
  }
}

// Draw spending trend line chart
inline void drawTrendLine(const std::vector<std::string> &labels,
                           const std::vector<double> &values, int height,
                           int width) {
  if (values.empty())
    return;

  double maxVal = *std::max_element(values.begin(), values.end());
  double minVal = *std::min_element(values.begin(), values.end());

  if (maxVal == minVal) {
    maxVal = minVal + 100; // Prevent flat line at zero
  }

  // Normalize values to height
  std::vector<int> normalized;
  for (double v : values) {
    int norm = static_cast<int>(((v - minVal) / (maxVal - minVal)) * (height - 1));
    normalized.push_back(norm);
  }

  // Draw from top to bottom
  for (int row = height - 1; row >= 0; row--) {
    // Y-axis label
    if (row == height - 1) {
      std::cout << "  " << std::setw(8) << std::right << std::fixed
                << std::setprecision(0) << maxVal << " |";
    } else if (row == 0) {
      std::cout << "  " << std::setw(8) << std::right << std::fixed
                << std::setprecision(0) << minVal << " |";
    } else {
      std::cout << "           |";
    }

    // Calculate x positions for each data point
    int numPoints = static_cast<int>(values.size());
    int chartWidth = 50;

    for (int col = 0; col < chartWidth; col++) {
      // Find which data point this column corresponds to
      int dataIdx = (col * (numPoints - 1)) / (chartWidth - 1);
      if (dataIdx >= numPoints)
        dataIdx = numPoints - 1;

      if (normalized[dataIdx] == row) {
        setColor(14); // Yellow
        std::cout << "●";
        resetColor();
      } else if (row < normalized[dataIdx]) {
        // Fill below the point
        setColor(6); // Dark yellow
        std::cout << "│";
        resetColor();
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }

  // X-axis
  std::cout << "           +";
  for (int i = 0; i < 50; i++) {
    std::cout << "-";
  }
  std::cout << std::endl;

  // Labels (show first, middle, last)
  std::cout << "            ";
  if (!labels.empty()) {
    std::cout << std::left << std::setw(20) << labels.front();
    if (labels.size() > 2) {
      std::cout << std::setw(15) << labels[labels.size() / 2];
    }
    if (labels.size() > 1) {
      std::cout << labels.back();
    }
  }
  std::cout << std::endl;
}

inline void showGraphsScreen() {
  clearScreen();

  // Beautiful header with consistent navigation
  drawScreenHeader("AI Expense - Spending Graphs", true);
  std::cout << std::endl;

  // Get all transactions
  std::vector<Transaction> transactions = TransactionManager::getAllTransactions();

  if (transactions.empty()) {
    drawInfoBox("📭 No transactions found yet!",
                "   Add some transactions to see your spending graphs.");
    
    drawNavFooter();
    drawPrompt("Enter command");
    std::string input = getInput();
    if (handleNavigation(input)) return;
    showMainMenu();
    return;
  }

  // Aggregate data by month
  std::map<std::string, double> monthlyExpenses;
  std::map<std::string, double> monthlyIncome;
  std::map<std::string, double> categorySpending;

  // Category keywords
  auto categorizeExpense = [](const std::string &description) -> std::string {
    std::string desc = description;
    std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);

    if (desc.find("food") != std::string::npos ||
        desc.find("restaurant") != std::string::npos ||
        desc.find("grocery") != std::string::npos ||
        desc.find("cafe") != std::string::npos ||
        desc.find("dining") != std::string::npos ||
        desc.find("meal") != std::string::npos ||
        desc.find("donut") != std::string::npos) {
      return "Food";
    } else if (desc.find("transport") != std::string::npos ||
               desc.find("uber") != std::string::npos ||
               desc.find("taxi") != std::string::npos ||
               desc.find("gas") != std::string::npos ||
               desc.find("fuel") != std::string::npos ||
               desc.find("bus") != std::string::npos ||
               desc.find("train") != std::string::npos) {
      return "Transport";
    } else if (desc.find("rent") != std::string::npos ||
               desc.find("housing") != std::string::npos ||
               desc.find("mortgage") != std::string::npos) {
      return "Rent";
    } else if (desc.find("utility") != std::string::npos ||
               desc.find("electric") != std::string::npos ||
               desc.find("water") != std::string::npos ||
               desc.find("internet") != std::string::npos ||
               desc.find("phone") != std::string::npos) {
      return "Utilities";
    }
    return "Other";
  };

  for (const auto &t : transactions) {
    int month, year;
    if (parseDateToMonthYear(t.getDate(), month, year)) {
      std::string key = getMonthName(month) + " " + std::to_string(year % 100);

      if (t.getType() == "expense") {
        monthlyExpenses[key] += t.getAmount();
        std::string category = categorizeExpense(t.getDescription());
        categorySpending[category] += t.getAmount();
      } else if (t.getType() == "income") {
        monthlyIncome[key] += t.getAmount();
      }
    }
  }

  // Calculate totals
  double totalExpenses = TransactionManager::getTotalExpenses();
  double totalIncome = TransactionManager::getTotalIncome();

  // Display summary in a nice box
  std::cout << "  ┌";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┐" << std::endl;
  
  std::cout << "  │                         📊 Financial Summary";
  for (int i = 0; i < 27; i++) std::cout << " ";
  std::cout << "│" << std::endl;
  
  std::cout << "  ├";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┤" << std::endl;
  
  std::cout << "  │  Total Income:   ";
  setColor(10);
  std::ostringstream incStr;
  incStr << "+$" << std::fixed << std::setprecision(2) << totalIncome;
  std::cout << std::setw(14) << incStr.str();
  resetColor();
  for (int i = 0; i < 40; i++) std::cout << " ";
  std::cout << "│" << std::endl;

  std::cout << "  │  Total Expenses: ";
  setColor(12);
  std::ostringstream expTotalStr;
  expTotalStr << "-$" << std::fixed << std::setprecision(2) << totalExpenses;
  std::cout << std::setw(14) << expTotalStr.str();
  resetColor();
  for (int i = 0; i < 40; i++) std::cout << " ";
  std::cout << "│" << std::endl;

  double balance = totalIncome - totalExpenses;
  std::cout << "  │  Net Balance:    ";
  if (balance >= 0) {
    setColor(10);
    std::ostringstream balStr;
    balStr << "+$" << std::fixed << std::setprecision(2) << balance;
    std::cout << std::setw(14) << balStr.str();
  } else {
    setColor(12);
    std::ostringstream balStr;
    balStr << "-$" << std::fixed << std::setprecision(2) << (-balance);
    std::cout << std::setw(14) << balStr.str();
  }
  resetColor();
  for (int i = 0; i < 40; i++) std::cout << " ";
  std::cout << "│" << std::endl;
  
  std::cout << "  └";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┘" << std::endl;
  std::cout << std::endl;

  // Menu for different graph types
  drawSectionTitle("Select a Graph to View", "📈");
  std::cout << std::endl;

  drawMenuOption("1", "Category Breakdown (Pie Chart)", "🥧");
  drawMenuOption("2", "Monthly Comparison (Bar Chart)", "📊");
  drawMenuOption("3", "Spending Trend (Line Chart)", "📉");
  drawMenuOption("4", "Top Expenses (Horizontal Bars)", "💰");
  drawMenuOption("5", "All Graphs Overview", "🔍");
  std::cout << std::endl;
  drawMenuOption("b", "Go Back to Menu", "←");

  drawPrompt("Choice");
  std::string choice = getInput();

  // Handle navigation
  if (choice == "b" || choice == "B" || choice == "back" || choice == "m" || choice == "M") {
    showMainMenu();
    return;
  }
  if (handleNavigation(choice)) return;

  clearScreen();
  drawScreenHeader("AI Expense - Spending Graphs", true);
  std::cout << std::endl;

  if (choice == "1") {
    // Category breakdown pie chart
    drawSectionTitle("Spending by Category", "🥧");

    drawPieChartASCII(categorySpending, totalExpenses);

  } else if (choice == "2") {
    // Monthly comparison bar chart
    drawSectionTitle("Monthly Income vs Expenses", "📊");
    std::cout << std::endl;

    // Prepare data for vertical bars
    std::vector<std::string> labels;
    std::vector<double> expenses;
    std::vector<double> incomes;

    for (const auto &pair : monthlyExpenses) {
      labels.push_back(pair.first);
      expenses.push_back(pair.second);
      incomes.push_back(monthlyIncome[pair.first]);
    }

    if (!labels.empty()) {
      drawVerticalBars(labels, expenses, incomes, 12);
    } else {
      std::cout << "  No monthly data available." << std::endl;
    }

  } else if (choice == "3") {
    // Spending trend line chart
    drawSectionTitle("Expense Trend Over Time", "📉");
    std::cout << std::endl;

    // Sort transactions by date and create cumulative spending
    std::vector<std::pair<std::string, double>> dailyExpenses;
    std::map<std::string, double> dateExpenses;

    for (const auto &t : transactions) {
      if (t.getType() == "expense") {
        dateExpenses[t.getDate()] += t.getAmount();
      }
    }

    for (const auto &pair : dateExpenses) {
      dailyExpenses.push_back(pair);
    }

    if (!dailyExpenses.empty()) {
      std::vector<std::string> labels;
      std::vector<double> values;

      double cumulative = 0;
      for (const auto &pair : dailyExpenses) {
        cumulative += pair.second;
        labels.push_back(pair.first);
        values.push_back(cumulative);
      }

      drawTrendLine(labels, values, 10, 50);

      std::cout << std::endl;
      std::cout << "  * Chart shows cumulative spending over time" << std::endl;
    } else {
      std::cout << "  No expense data available." << std::endl;
    }

  } else if (choice == "4") {
    // Top expenses horizontal bar chart
    drawSectionTitle("Top Spending Categories", "💰");
    std::cout << std::endl;

    // Sort categories by spending
    std::vector<std::pair<std::string, double>> sorted(categorySpending.begin(),
                                                        categorySpending.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });

    double maxSpending = sorted.empty() ? 0 : sorted[0].second;

    for (const auto &item : sorted) {
      drawHorizontalBar(item.first, item.second, maxSpending, 30, true);
    }

    // Also show income
    std::cout << std::endl;
    drawSectionTitle("Income Sources", "📈");

    // Categorize income (simplified)
    std::map<std::string, double> incomeCategories;
    for (const auto &t : transactions) {
      if (t.getType() == "income") {
        incomeCategories["Income"] += t.getAmount();
      }
    }

    double maxIncome =
        incomeCategories.empty() ? 0 : incomeCategories.begin()->second;
    for (const auto &item : incomeCategories) {
      drawHorizontalBar(item.first, item.second, maxIncome, 30, false);
    }

  } else if (choice == "5") {
    // All graphs overview
    drawSectionTitle("Complete Financial Overview", "🔍");
    std::cout << std::endl;

    // Mini category breakdown
    std::cout << "  ┌─ Category Breakdown ─────────────────────────────┐"
              << std::endl;
    for (const auto &item : categorySpending) {
      double pct = totalExpenses > 0 ? (item.second / totalExpenses) * 100 : 0;
      int barLen = static_cast<int>(pct / 5); // Scale to fit

      std::cout << "  │ " << std::left << std::setw(12) << item.first;
      setColor(14);
      for (int i = 0; i < barLen; i++)
        std::cout << "▓";
      resetColor();
      for (int i = barLen; i < 20; i++)
        std::cout << "░";
      std::cout << " " << std::fixed << std::setprecision(1) << pct << "%"
                << std::setw(5) << "" << "│" << std::endl;
    }
    std::cout << "  └──────────────────────────────────────────────────┘"
              << std::endl;
    std::cout << std::endl;

    // Mini monthly summary
    std::cout << "  ┌─ Monthly Summary ────────────────────────────────┐"
              << std::endl;
    for (const auto &pair : monthlyExpenses) {
      std::cout << "  │ " << std::left << std::setw(10) << pair.first;
      setColor(12);
      std::cout << " Exp: $" << std::setw(10) << std::fixed
                << std::setprecision(2) << pair.second;
      resetColor();
      setColor(10);
      std::cout << " Inc: $" << std::setw(10) << monthlyIncome[pair.first];
      resetColor();
      std::cout << " │" << std::endl;
    }
    std::cout << "  └──────────────────────────────────────────────────┘"
              << std::endl;
    std::cout << std::endl;

    // Savings rate
    double savingsRate =
        totalIncome > 0 ? ((totalIncome - totalExpenses) / totalIncome) * 100 : 0;
    std::cout << "  📈 Savings Rate: ";
    if (savingsRate >= 20) {
      setColor(10); // Green - good
    } else if (savingsRate >= 0) {
      setColor(14); // Yellow - okay
    } else {
      setColor(12); // Red - overspending
    }
    std::cout << std::fixed << std::setprecision(1) << savingsRate << "%"
              << std::endl;
    resetColor();

    if (savingsRate >= 20) {
      std::cout << "  ✅ Great job! You're saving well." << std::endl;
    } else if (savingsRate >= 0) {
      std::cout << "  ⚠️  Consider increasing your savings." << std::endl;
    } else {
      std::cout << "  ❌ Warning: You're spending more than you earn!"
                << std::endl;
    }

  } else {
    std::cout << "  Invalid choice." << std::endl;
  }

  // Consistent navigation footer
  drawNavFooter();

  drawPrompt("Press ENTER to continue or 'b' to go back");
  std::string input = getInput();
  if (input == "b" || input == "B" || input == "back") {
    showMainMenu();
    return;
  }
  if (handleNavigation(input)) return;
  showGraphsScreen();
}
