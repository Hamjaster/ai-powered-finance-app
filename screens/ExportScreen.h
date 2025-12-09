#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include "../modules/Transaction.h"
#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

// Generate a timestamp for filenames
inline std::string getTimestamp() {
  time_t now = time(0);
  tm *timeinfo = localtime(&now);
  
  std::ostringstream oss;
  oss << (1900 + timeinfo->tm_year)
      << std::setfill('0') << std::setw(2) << (timeinfo->tm_mon + 1)
      << std::setw(2) << timeinfo->tm_mday << "_"
      << std::setw(2) << timeinfo->tm_hour
      << std::setw(2) << timeinfo->tm_min
      << std::setw(2) << timeinfo->tm_sec;
  return oss.str();
}

// Export transactions to CSV format
inline bool exportToCSV(const std::vector<Transaction> &transactions, const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  // CSV Header
  file << "ID,Date,Type,Amount,Description\n";

  // Write each transaction
  for (const auto &t : transactions) {
    // Escape description (handle commas and quotes)
    std::string desc = t.getDescription();
    bool needsQuotes = desc.find(',') != std::string::npos || 
                       desc.find('"') != std::string::npos ||
                       desc.find('\n') != std::string::npos;
    
    if (needsQuotes) {
      // Escape quotes by doubling them
      std::string escaped;
      for (char c : desc) {
        if (c == '"') escaped += "\"\"";
        else escaped += c;
      }
      desc = "\"" + escaped + "\"";
    }

    file << t.getId() << ","
         << t.getDate() << ","
         << t.getType() << ","
         << std::fixed << std::setprecision(2) << t.getAmount() << ","
         << desc << "\n";
  }

  file.close();
  return true;
}

// Export transactions to plain text format
inline bool exportToText(const std::vector<Transaction> &transactions, const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  // Header
  file << "═══════════════════════════════════════════════════════════════════════\n";
  file << "                    AI EXPENSE MANAGER - TRANSACTION REPORT\n";
  file << "                    Generated: " << Transaction::generateCurrentDate() << "\n";
  file << "═══════════════════════════════════════════════════════════════════════\n\n";

  // Summary
  double totalIncome = 0, totalExpenses = 0;
  for (const auto &t : transactions) {
    if (t.getType() == "income") totalIncome += t.getAmount();
    else totalExpenses += t.getAmount();
  }

  file << "SUMMARY\n";
  file << "───────────────────────────────────────\n";
  file << "Total Transactions: " << transactions.size() << "\n";
  file << "Total Income:       $" << std::fixed << std::setprecision(2) << totalIncome << "\n";
  file << "Total Expenses:     $" << totalExpenses << "\n";
  file << "Net Balance:        $" << (totalIncome - totalExpenses) << "\n\n";

  // Transactions list
  file << "TRANSACTIONS\n";
  file << "───────────────────────────────────────────────────────────────────────\n";
  file << std::left << std::setw(6) << "ID" 
       << std::setw(14) << "Date" 
       << std::setw(10) << "Type" 
       << std::setw(14) << "Amount" 
       << "Description\n";
  file << "───────────────────────────────────────────────────────────────────────\n";

  for (const auto &t : transactions) {
    std::string amountStr = (t.getType() == "income" ? "+$" : "-$") + 
                            std::to_string(t.getAmount()).substr(0, std::to_string(t.getAmount()).find('.') + 3);
    
    file << std::left << std::setw(6) << t.getId()
         << std::setw(14) << t.getDate()
         << std::setw(10) << t.getType()
         << std::setw(14) << amountStr
         << t.getDescription() << "\n";
  }

  file << "───────────────────────────────────────────────────────────────────────\n";
  file << "\nEnd of Report\n";

  file.close();
  return true;
}

// Export transactions to JSON format
inline bool exportToJSON(const std::vector<Transaction> &transactions, const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  json j;
  j["exported_at"] = Transaction::generateCurrentDate();
  j["total_transactions"] = transactions.size();
  
  json trans_array = json::array();
  for (const auto &t : transactions) {
    trans_array.push_back(t.toJson());
  }
  j["transactions"] = trans_array;

  file << j.dump(2);
  file.close();
  return true;
}

inline void showExportScreen() {
  clearScreen();

  // Header
  drawScreenHeader("AI Expense - Export Data", true);
  std::cout << std::endl;

  // Get transactions
  std::vector<Transaction> transactions = TransactionManager::getAllTransactions();

  if (transactions.empty()) {
    drawInfoBox("📭 No transactions to export!",
                "   Add some transactions first.");
    
    drawNavFooter();
    drawPrompt("Press ENTER to go back");
    getInput();
    showMainMenu();
    return;
  }

  // Info box
  drawInfoBox("📤 Export your transaction data to a file",
              "   Files will be saved in the current directory.");
  std::cout << std::endl;

  // Show transaction count
  drawInfoLine("📊", "Transactions to export", std::to_string(transactions.size()), 14);
  std::cout << std::endl;

  // Export format options
  drawSectionTitle("Select Export Format", "📁");
  std::cout << std::endl;

  drawMenuOption("1", "CSV (Spreadsheet compatible)", "📊");
  drawMenuOption("2", "Text Report (Human readable)", "📄");
  drawMenuOption("3", "JSON (Data interchange)", "🔧");
  drawMenuOption("4", "All Formats", "📦");
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

  std::cout << std::endl;

  std::string timestamp = getTimestamp();
  bool success = false;
  std::vector<std::string> exportedFiles;

  if (choice == "1" || choice == "4") {
    std::string filename = "transactions_" + timestamp + ".csv";
    if (exportToCSV(transactions, filename)) {
      exportedFiles.push_back(filename);
      success = true;
    }
  }

  if (choice == "2" || choice == "4") {
    std::string filename = "transactions_" + timestamp + ".txt";
    if (exportToText(transactions, filename)) {
      exportedFiles.push_back(filename);
      success = true;
    }
  }

  if (choice == "3" || choice == "4") {
    std::string filename = "transactions_" + timestamp + ".json";
    if (exportToJSON(transactions, filename)) {
      exportedFiles.push_back(filename);
      success = true;
    }
  }

  if (choice != "1" && choice != "2" && choice != "3" && choice != "4") {
    drawStatusMessage("Invalid choice.", "error");
  } else if (success) {
    std::cout << "  ┌";
    for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
    std::cout << "┐" << std::endl;
    
    std::string successMsg = "✓ Export successful!";
    int successPad = BOX_INNER - static_cast<int>(successMsg.length());
    std::cout << "  │ " << successMsg;
    for (int i = 0; i < successPad; i++) std::cout << " ";
    std::cout << " │" << std::endl;
    
    std::cout << "  ├";
    for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
    std::cout << "┤" << std::endl;
    
    for (const auto &file : exportedFiles) {
      std::string fileEntry = "📁 " + file;
      int filePad = BOX_INNER - static_cast<int>(fileEntry.length());
      if (filePad < 0) filePad = 0;
      std::cout << "  │ " << fileEntry;
      for (int i = 0; i < filePad; i++) std::cout << " ";
      std::cout << " │" << std::endl;
    }
    
    std::cout << "  └";
    for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
    std::cout << "┘" << std::endl;
  } else {
    drawStatusMessage("Export failed. Check file permissions.", "error");
  }

  drawNavFooter();
  drawPrompt("Press ENTER to continue");
  getInput();
  showMainMenu();
}
