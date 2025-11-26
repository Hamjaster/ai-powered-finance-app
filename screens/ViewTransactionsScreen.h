#pragma once

#include <iomanip>
#include <iostream>
#include <vector>

#include "../modules/Transaction.h"
#include "../modules/TransactionManager.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"

inline void showViewTransactionsScreen()
{
  clearScreen();

  drawBoxedTitle("View Transactions");
  std::cout << std::endl;

  std::vector<Transaction> transactions = TransactionManager::getAllTransactions();

  if (transactions.empty())
  {
    std::cout << "No transactions found." << std::endl;
    std::cout << std::endl;
  }
  else
  {
    for (int i = static_cast<int>(transactions.size()) - 1; i >= 0; i--)
    {
      const Transaction &t = transactions[i];

      if (t.getType() == "income")
      {
        setColor(10);
      }
      else
      {
        setColor(12);
      }

      std::cout << "[" << t.getId() << "] ";
      resetColor();

      std::cout << t.getDate() << " - ";

      if (t.getType() == "income")
      {
        setColor(10);
        std::cout << "+$";
      }
      else
      {
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

    std::cout << "Summary:" << std::endl;
    setColor(10);
    std::cout << "Total Income: $" << std::fixed << std::setprecision(2) << totalIncome << std::endl;
    resetColor();
    setColor(12);
    std::cout << "Total Expenses: $" << std::fixed << std::setprecision(2) << totalExpenses << std::endl;
    resetColor();
    std::cout << "Balance: $";
    if (balance >= 0)
    {
      setColor(10);
    }
    else
    {
      setColor(12);
    }
    std::cout << std::fixed << std::setprecision(2) << balance << std::endl;
    resetColor();
    std::cout << std::endl;
  }

  std::cout << "Press ";
  setColor(14);
  std::cout << "ENTER";
  resetColor();
  std::cout << " to return..." << std::endl;
  std::cin.ignore();
  std::cin.get();
  showMainMenu();
}

