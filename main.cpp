#include "modules/AuthManager.h"
#include "modules/FileHandler.h"
#include "modules/Transaction.h"
#include "modules/TransactionManager.h"
#include "modules/User.h"
#include <conio.h> // For _getch() on Windows
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h> // For Windows console functions


using namespace std;

// Function to clear screen
void clearScreen() { system("cls"); }

// Function to set console color (Windows)
void setColor(int color) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color);
}

// Function to reset console color
void resetColor() {
  setColor(7); // White
}

// Function to draw boxed title
void drawBoxedTitle(const string &title) {
  int width = title.length() + 4;
  cout << "+";
  for (int i = 0; i < width - 2; i++)
    cout << "-";
  cout << "+\n";
  cout << "| " << title << " |\n";
  cout << "+";
  for (int i = 0; i < width - 2; i++)
    cout << "-";
  cout << "+\n";
}

// Function to get password input (masked)
string getPasswordInput() {
  string password = "";
  char ch;

  while (true) {
    ch = _getch();

    if (ch == '\r' || ch == '\n') { // Enter key
      cout << endl;
      break;
    } else if (ch == '\b' || ch == 127) { // Backspace
      if (password.length() > 0) {
        password.pop_back();
        cout << "\b \b";
      }
    } else {
      password += ch;
      cout << "*";
    }
  }

  return password;
}

// Function to get regular input
string getInput() {
  string input;
  getline(cin, input);
  // Clear any remaining newline
  if (input.empty() && cin.peek() == '\n') {
    cin.ignore();
  }
  return input;
}

// Function to get double input
double getDoubleInput() {
  string input = getInput();
  try {
    return stod(input);
  } catch (...) {
    return -1.0; // Invalid input
  }
}

// Forward declarations
void showMainMenu();
void showViewTransactionsScreen();
void showAddTransactionScreen();

// Setup Screen
void showSetupScreen() {
  clearScreen();

  // Boxed title
  drawBoxedTitle("AI Expense Manager • Setup");
  cout << endl;

  // Welcome message
  cout << "👋 Welcome! ";
  setColor(11); // Light blue/cyan
  cout << "Let's";
  resetColor();
  cout << " set up your profile." << endl;
  cout << endl;

  // Get username
  cout << "Enter your name: ";
  string username = getInput();

  // Get password
  cout << "Create a password: ";
  string password = getPasswordInput();

  // Get password confirmation
  cout << "Confirm password: ";
  string confirmPassword = getPasswordInput();

  cout << endl;

  // Try to setup user
  if (AuthManager::setupUser(username, password, confirmPassword)) {
    // Success message
    setColor(10); // Green
    cout << "✓ Profile created successfully!" << endl;
    resetColor();
    cout << endl;

    cout << "Next time, just enter your ";
    setColor(11); // Light blue/cyan
    cout << "name";
    resetColor();
    cout << " + ";
    setColor(11);
    cout << "password";
    resetColor();
    cout << " to continue." << endl;
    cout << endl;

    cout << "Press ";
    setColor(14); // Yellow/Orange
    cout << "ENTER";
    resetColor();
    cout << " to launch your dashboard..." << endl;

    // Wait for ENTER
    cin.ignore();
    cin.get();

    // Navigate to main menu
    showMainMenu();
  } else {
    // Password mismatch
    setColor(12); // Red
    cout << "✗ Passwords do not match. Please try again." << endl;
    resetColor();
    cout << endl;
    cout << "Press ENTER to retry..." << endl;
    cin.get();
    showSetupScreen(); // Retry
  }
}

// Login Screen
void showLoginScreen() {
  clearScreen();

  // Boxed title
  drawBoxedTitle("AI Expense Manager • Login");
  cout << endl;

  // Load user to get username
  User storedUser = FileHandler::readUserFromFile();
  string username = storedUser.getUsername();

  // Welcome message and display name
  cout << "Hy, " << username << endl;
  cout << "Name: " << username << endl;

  // Get password
  cout << "Password: ";
  string password = getPasswordInput();

  cout << endl;

  // Try to login
  if (AuthManager::login(password)) {
    // Success message
    setColor(10); // Green
    cout << "✓ Login successful. Loading your dashboard..." << endl;
    resetColor();

    // Small delay for effect
    Sleep(1500);

    // Navigate to main menu
    showMainMenu();
  } else {
    // Failed login
    setColor(12); // Red
    cout << "✗ Invalid password. Please try again." << endl;
    resetColor();
    cout << endl;
    cout << "Press ENTER to retry..." << endl;
    cin.get();
    showLoginScreen(); // Retry
  }
}

// View Transactions Screen
void showViewTransactionsScreen() {
  clearScreen();

  drawBoxedTitle("View Transactions");
  cout << endl;

  vector<Transaction> transactions = TransactionManager::getAllTransactions();

  if (transactions.empty()) {
    cout << "No transactions found." << endl;
    cout << endl;
  } else {
    // Display transactions in reverse order (newest first)
    for (int i = transactions.size() - 1; i >= 0; i--) {
      const Transaction &t = transactions[i];

      // Color based on type
      if (t.getType() == "income") {
        setColor(10); // Green for income
      } else {
        setColor(12); // Red for expense
      }

      cout << "[" << t.getId() << "] ";
      resetColor();

      cout << t.getDate() << " - ";

      if (t.getType() == "income") {
        setColor(10);
        cout << "+$";
      } else {
        setColor(12);
        cout << "-$";
      }

      cout << fixed << setprecision(2) << t.getAmount();
      resetColor();

      cout << " - " << t.getDescription() << endl;
    }
    cout << endl;

    // Show summary
    double totalIncome = TransactionManager::getTotalIncome();
    double totalExpenses = TransactionManager::getTotalExpenses();
    double balance = TransactionManager::getBalance();

    cout << "Summary:" << endl;
    setColor(10);
    cout << "Total Income: $" << fixed << setprecision(2) << totalIncome
         << endl;
    resetColor();
    setColor(12);
    cout << "Total Expenses: $" << fixed << setprecision(2) << totalExpenses
         << endl;
    resetColor();
    cout << "Balance: $";
    if (balance >= 0) {
      setColor(10);
    } else {
      setColor(12);
    }
    cout << fixed << setprecision(2) << balance << endl;
    resetColor();
    cout << endl;
  }

  cout << "Press ";
  setColor(14);
  cout << "ENTER";
  resetColor();
  cout << " to return..." << endl;
  cin.ignore();
  cin.get();
  showMainMenu();
}

// Add Transaction Screen
void showAddTransactionScreen() {
  clearScreen();

  drawBoxedTitle("Add Transaction");
  cout << endl;

  // Get transaction type
  cout << "Type: ";
  setColor(12);
  cout << "(1) Expense";
  resetColor();
  cout << " ";
  setColor(10);
  cout << "(2) Income";
  resetColor();
  cout << endl;
  cout << "Choice: ";
  string choiceStr = getInput();
  int choice = (choiceStr == "1" || choiceStr == "2") ? stoi(choiceStr) : 0;

  string type;
  if (choice == 1) {
    type = "expense";
  } else if (choice == 2) {
    type = "income";
  } else {
    setColor(12);
    cout << "✗ Invalid choice." << endl;
    resetColor();
    cout << endl;
    cout << "Press ENTER to retry..." << endl;
    cin.get();
    showAddTransactionScreen();
    return;
  }

  // Get amount
  cout << "Amount: $";
  double amount = getDoubleInput();
  if (amount <= 0) {
    setColor(12);
    cout << "✗ Invalid amount." << endl;
    resetColor();
    cout << endl;
    cout << "Press ENTER to retry..." << endl;
    cin.get();
    showAddTransactionScreen();
    return;
  }

  // Get description
  cout << "Description: ";
  string description = getInput();
  if (description.empty()) {
    setColor(12);
    cout << "✗ Description cannot be empty." << endl;
    resetColor();
    cout << endl;
    cout << "Press ENTER to retry..." << endl;
    cin.get();
    showAddTransactionScreen();
    return;
  }

  // Optional note
  cout << "Note (optional): ";
  string note = getInput();
  if (!note.empty()) {
    description += " - " + note;
  }

  cout << endl;

  // Add transaction
  if (TransactionManager::addTransaction(type, amount, description)) {
    setColor(10);
    cout << "✓ ";
    if (type == "income") {
      cout << "Income";
    } else {
      cout << "Expense";
    }
    cout << " added successfully!" << endl;
    resetColor();
    cout << endl;
  } else {
    setColor(12);
    cout << "✗ Failed to add transaction." << endl;
    resetColor();
    cout << endl;
  }

  cout << "Press ";
  setColor(14);
  cout << "ENTER";
  resetColor();
  cout << " to return to menu..." << endl;
  cin.ignore();
  cin.get();
  showMainMenu();
}

// Main Menu Screen
void showMainMenu() {
  clearScreen();

  drawBoxedTitle("AI Expense • Dashboard");
  cout << endl;

  // Show quick summary
  double balance = TransactionManager::getBalance();
  double totalIncome = TransactionManager::getTotalIncome();
  double totalExpenses = TransactionManager::getTotalExpenses();

  cout << "💰 Balance: $";
  if (balance >= 0) {
    setColor(10);
  } else {
    setColor(12);
  }
  cout << fixed << setprecision(2) << balance << endl;
  resetColor();
  cout << endl;

  // Menu options
  cout << "Commands:" << endl;
  cout << "  [";
  setColor(11);
  cout << "t";
  resetColor();
  cout << "]ransact - Add new transaction" << endl;
  cout << "  [";
  setColor(11);
  cout << "v";
  resetColor();
  cout << "]iew - View all transactions" << endl;
  cout << "  [";
  setColor(11);
  cout << "q";
  resetColor();
  cout << "]uit - Exit application" << endl;
  cout << endl;
  cout << "Enter command: ";

  string command = getInput();

  if (command == "t" || command == "T" || command == "transact") {
    showAddTransactionScreen();
  } else if (command == "v" || command == "V" || command == "view") {
    showViewTransactionsScreen();
  } else if (command == "q" || command == "Q" || command == "quit") {
    clearScreen();
    cout << "Thank you for using AI Expense Manager!" << endl;
    exit(0);
  } else {
    cout << "Invalid command. Press ENTER to continue..." << endl;
    cin.get();
    showMainMenu();
  }
}

// Main function
int main() {
  // Ensure data directory exists
  FileHandler::ensureDataDirectory();

  // Check if first time or returning user
  if (AuthManager::isFirstTime()) {
    showSetupScreen();
  } else {
    showLoginScreen();
  }

  // After successful authentication, dashboard will be shown
  // (to be implemented next)

  return 0;
}
