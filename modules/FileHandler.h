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
    // _access(path, mode). mode = 0 just checks for existence. returns 0 if
    // exists, -1 if not.
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
    ifstream file(USER_FILE);
    if (!file.is_open())
    {
      return User(); // Return empty user
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string jsonContent = buffer.str();

    file.close();
    return User::fromJson(jsonContent);
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
