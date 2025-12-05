#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * EncryptionManager - XOR-based encryption/decryption
 * 
 * HOW XOR ENCRYPTION WORKS:
 * =========================
 * 
 * XOR (Exclusive OR) is a bitwise operation that works like this:
 * - 0 XOR 0 = 0
 * - 0 XOR 1 = 1
 * - 1 XOR 0 = 1
 * - 1 XOR 1 = 0
 * 
 * The key property: (A XOR K) XOR K = A
 * This means if you XOR data with a key, then XOR again with the same key,
 * you get the original data back!
 * 
 * EXAMPLE:
 * --------
 * Plaintext:  "Hello" (bytes: 72, 101, 108, 108, 111)
 * Key:        "key"   (bytes: 107, 101, 121)
 * 
 * Step 1: XOR each plaintext byte with corresponding key byte (cycle through key)
 *   H (72)  XOR k (107) = 35
 *   e (101) XOR e (101) = 0
 *   l (108) XOR y (121) = 21
 *   l (108) XOR k (107) = 7
 *   o (111) XOR e (101) = 10
 * 
 * Encrypted: [35, 0, 21, 7, 10] (unreadable binary data)
 * 
 * Step 2: To decrypt, XOR encrypted bytes with the same key again
 *   35  XOR k (107) = 72  (H)
 *   0   XOR e (101) = 101 (e)
 *   21  XOR y (121) = 108 (l)
 *   7   XOR k (107) = 108 (l)
 *   10  XOR e (101) = 111 (o)
 * 
 * Result: "Hello" (original data restored!)
 * 
 * IMPLEMENTATION:
 * ---------------
 * 1. Convert password to encryption key (use password directly or derive from it)
 * 2. For each byte in plaintext, XOR with corresponding byte in key
 * 3. If key is shorter than data, cycle through key (key[i % key.length()])
 * 4. Store encrypted bytes (may contain non-printable characters)
 * 5. To decrypt, XOR encrypted data with same key again
 */
class EncryptionManager
{
public:
  /**
   * Encrypt plaintext using XOR with password-derived key
   * @param plaintext The data to encrypt
   * @param password The password to derive encryption key from
   * @return Encrypted data as string (may contain binary characters)
   */
  static string encrypt(const string &plaintext, const string &password)
  {
    if (password.empty())
    {
      return plaintext; // No encryption if no password
    }

    string encrypted;
    encrypted.reserve(plaintext.length());

    // XOR each byte of plaintext with corresponding byte of password key
    // Cycle through password if it's shorter than plaintext
    for (size_t i = 0; i < plaintext.length(); i++)
    {
      char keyChar = password[i % password.length()];
      char encryptedChar = plaintext[i] ^ keyChar; // XOR operation
      encrypted += encryptedChar;
    }

    return encrypted;
  }

  /**
   * Decrypt encrypted data using XOR with password-derived key
   * @param encrypted The encrypted data
   * @param password The password to derive decryption key from
   * @return Decrypted plaintext
   */
  static string decrypt(const string &encrypted, const string &password)
  {
    if (password.empty())
    {
      return encrypted; // No decryption if no password
    }

    // XOR encryption is symmetric: encrypt and decrypt use the same operation!
    // (A XOR K) XOR K = A
    return encrypt(encrypted, password);
  }

  /**
   * Convert encrypted binary data to base64-like hex string for safe storage
   * This is optional - we can store binary directly, but hex is more readable
   * @param data Binary data
   * @return Hex string representation
   */
  static string toHex(const string &data)
  {
    const char hexChars[] = "0123456789ABCDEF";
    string hex;
    hex.reserve(data.length() * 2);

    for (unsigned char c : data)
    {
      hex += hexChars[(c >> 4) & 0xF]; // Upper 4 bits
      hex += hexChars[c & 0xF];         // Lower 4 bits
    }

    return hex;
  }

  /**
   * Convert hex string back to binary data
   * @param hex Hex string representation
   * @return Binary data
   */
  static string fromHex(const string &hex)
  {
    string data;
    data.reserve(hex.length() / 2);

    for (size_t i = 0; i < hex.length(); i += 2)
    {
      if (i + 1 < hex.length())
      {
        char high = hex[i];
        char low = hex[i + 1];

        int value = 0;
        if (high >= '0' && high <= '9')
          value += (high - '0') << 4;
        else if (high >= 'A' && high <= 'F')
          value += (high - 'A' + 10) << 4;

        if (low >= '0' && low <= '9')
          value += (low - '0');
        else if (low >= 'A' && low <= 'F')
          value += (low - 'A' + 10);

        data += static_cast<char>(value);
      }
    }

    return data;
  }
};




