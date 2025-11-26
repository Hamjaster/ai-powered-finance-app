#pragma once
#include "../json.hpp"
#include "Transaction.h"
#include "User.h"
#include <direct.h> // _mkdir
#include <fstream>
#include <io.h> // _access
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using json = nlohmann::json;
using namespace std;

class FileHandler
{
public:
  // Folder + file paths
  static inline const string DATA_DIR = "data";
  static inline const string USER_FILE = "data/user.json";
  static inline const string TRANSACTIONS_FILE = "data/transactions.json";

  // Create /data directory if missing
  static void ensureDataDirectory()
  {
    if (_access(DATA_DIR.c_str(), 0) != 0)
    {
      _mkdir(DATA_DIR.c_str());
    }
  }

  // Check if user.json exists
  static bool userFileExists()
  {
    ifstream file(USER_FILE);
    return file.good();
  }

  // Read user from JSON file
  static User readUserFromFile()
  {
    ifstream file(USER_FILE); // input from "data/user.json"
    if (!file.is_open())
    {
      cout << "check file open, if no return user obj";
      return User(); // Return empty user
    }
    stringstream buffer;
    buffer << file.rdbuf();
    // buffer = 94054309
    string jsonContent = buffer.str();
    // jsonContent = username=hamza, password=hamza1122

    file.close();
    return User::fromJson(jsonContent);
    // return {username : "hamza", password = "hamza1122"}
  }

  // Write user to file
  static void writeUserToFile(const User &user)
  {
    ofstream file(USER_FILE);
    if (!file.is_open())
    {
      cerr << "Error: Could not write to user file.\n";
      return;
    }

    file << user.toJson();
    file.close();
  }

  // -------- Transaction File Operations --------

  // Read all transactions from file
  static vector<Transaction> readTransactionsFromFile()
  {
    vector<Transaction> transactions;
    ifstream file(TRANSACTIONS_FILE);

    if (!file.is_open())
    {
      return transactions; // Return empty vector if file doesn't exist
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string jsonContent = buffer.str();
    file.close();

    if (jsonContent.empty())
    {
      return transactions;
    }

    try
    {
      json j = json::parse(jsonContent);
      if (j.contains("transactions") && j["transactions"].is_array())
      {
        for (const auto &item : j["transactions"])
        {
          transactions.push_back(Transaction::fromJson(item));
        }
      }
      // transactions<vector> = { { id, name, type, amount, date}, {}, {}, {} }
    }
    catch (const exception &e)
    {
      cerr << "Error parsing transactions file: " << e.what() << "\n";
    }

    return transactions;
  }

  // Write all transactions to file
  static void writeTransactionsToFile(const vector<Transaction> &transactions)
  {

    ofstream file(TRANSACTIONS_FILE);
    if (!file.is_open())
    {
      cerr << "Error: Could not write to transactions file.\n";
      return;
    }
    // input : transactions<vector>
    // {"transactions" = []}
    json j;
    j["transactions"] = json::array();

    for (const auto &transaction : transactions)
    {
      j["transactions"].push_back(transaction.toJson());
    }

    file << j.dump(4); // Pretty-printed JSON
    file.close();
  }
};
