#include "FileHandler.h"
#include "Transaction.h"
#include <algorithm>
#include <vector>

using namespace std;

class TransactionManager {
public:
  // Get all transactions
  static vector<Transaction> getAllTransactions() {
    return FileHandler::readTransactionsFromFile();
  }

  // Get next available ID
  static int getNextId() {
    vector<Transaction> transactions = getAllTransactions();
    if (transactions.empty()) {
      return 1;
    }

    int maxId = 0;
    for (const auto &t : transactions) {
      if (t.getId() > maxId) {
        maxId = t.getId();
      }
    }
    return maxId + 1;
  }

  // Add a new transaction
  static bool addTransaction(const string &type, double amount,
                             const string &description) {
    // Validate type
    if (type != "income" && type != "expense") {
      return false;
    }

    // Validate amount
    if (amount <= 0) {
      return false;
    }

    // Validate description
    if (description.empty()) {
      return false;
    }

    // Get all existing transactions
    vector<Transaction> transactions = getAllTransactions();

    // Create new transaction with auto-generated ID and date
    Transaction newTransaction(type, amount, description);
    newTransaction.setId(getNextId());

    // Add to vector
    transactions.push_back(newTransaction);

    // Save to file
    FileHandler::writeTransactionsToFile(transactions);

    return true;
  }

  // Get total income
  static double getTotalIncome() {
    vector<Transaction> transactions = getAllTransactions();
    double total = 0.0;
    for (const auto &t : transactions) {
      if (t.getType() == "income") {
        total += t.getAmount();
      }
    }
    return total;
  }

  // Get total expenses
  static double getTotalExpenses() {
    vector<Transaction> transactions = getAllTransactions();
    double total = 0.0;
    for (const auto &t : transactions) {
      if (t.getType() == "expense") {
        total += t.getAmount();
      }
    }
    return total;
  }

  // Get balance (income - expenses)
  static double getBalance() { return getTotalIncome() - getTotalExpenses(); }
};
