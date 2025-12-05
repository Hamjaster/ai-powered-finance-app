#pragma once
#include "../json.hpp"
#include "Transaction.h"
#include "User.h"
#include "EncryptionManager.h"
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

  // Read user from JSON file (with decryption)
  // @param password The password to decrypt the encrypted file
  static User readUserFromFile(const string &password)
  {
    ifstream file(USER_FILE); // input from "data/user.json"
    if (!file.is_open())
    {
      cout << "check file open, if no return user obj";
      return User(); // Return empty user
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string encryptedHex = buffer.str();
    file.close();

    // Decrypt the hex-encoded encrypted data
    string encryptedBinary = EncryptionManager::fromHex(encryptedHex);
    string jsonContent = EncryptionManager::decrypt(encryptedBinary, password);

    // Try to parse JSON - if it fails, password might be wrong
    try
    {
      return User::fromJson(jsonContent);
    }
    catch (const exception &e)
    {
      // If decryption failed (wrong password), return empty user
      cerr << "Error: Failed to decrypt user file. Wrong password?\n";
      return User();
    }
  }

  // Write user to file (with encryption)
  // @param user The user object to save
  // @param password The password to encrypt the file with
  static void writeUserToFile(const User &user, const string &password)
  {
    ofstream file(USER_FILE);
    if (!file.is_open())
    {
      cerr << "Error: Could not write to user file.\n";
      return;
    }

    // Get JSON string, encrypt it, then convert to hex for safe storage
    string jsonContent = user.toJson();
    string encrypted = EncryptionManager::encrypt(jsonContent, password);
    string encryptedHex = EncryptionManager::toHex(encrypted);

    // Write encrypted hex string to file
    file << encryptedHex;
    file.close();
  }

  // -------- Transaction File Operations --------

  // Read all transactions from file (with decryption)
  // @param password The password to decrypt the encrypted file
  static vector<Transaction> readTransactionsFromFile(const string &password)
  {
    vector<Transaction> transactions;
    ifstream file(TRANSACTIONS_FILE);

    if (!file.is_open())
    {
      return transactions; // Return empty vector if file doesn't exist
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string encryptedHex = buffer.str();
    file.close();

    if (encryptedHex.empty())
    {
      return transactions;
    }

    try
    {
      // Decrypt the hex-encoded encrypted data
      string encryptedBinary = EncryptionManager::fromHex(encryptedHex);
      string jsonContent = EncryptionManager::decrypt(encryptedBinary, password);

      // Parse the decrypted JSON
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
      // If decryption failed or parsing failed, return empty vector
      cerr << "Error parsing transactions file: " << e.what() << "\n";
    }

    return transactions;
  }

  // Write all transactions to file (with encryption)
  // @param transactions The transactions to save
  // @param password The password to encrypt the file with
  static void writeTransactionsToFile(const vector<Transaction> &transactions, const string &password)
  {
    ofstream file(TRANSACTIONS_FILE);
    if (!file.is_open())
    {
      cerr << "Error: Could not write to transactions file.\n";
      return;
    }

    // Build JSON structure
    // input : transactions<vector>
    // {"transactions" = []}
    json j;
    j["transactions"] = json::array();

    for (const auto &transaction : transactions)
    {
      j["transactions"].push_back(transaction.toJson());
    }

    // Encrypt the JSON string, then convert to hex for safe storage
    string jsonContent = j.dump(4); // Pretty-printed JSON
    string encrypted = EncryptionManager::encrypt(jsonContent, password);
    string encryptedHex = EncryptionManager::toHex(encrypted);

    // Write encrypted hex string to file
    file << encryptedHex;
    file.close();
  }
};
