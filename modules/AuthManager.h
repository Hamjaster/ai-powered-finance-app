#pragma once
#include "User.h"
#include "FileHandler.h"
using namespace std;

class AuthManager
{
public:
    // Check if first time user
    static bool isFirstTime()
    {
        return !FileHandler::userFileExists();
    }

    // Setup new user
    static bool setupUser(const string &username, const string &password, const string &confirmPassword)
    {
        // Check if passwords match
        if (password != confirmPassword)
        {
            return false;
        }

        // Create user and save (encrypt with password)
        User newUser(username, password);
        FileHandler::ensureDataDirectory();
        FileHandler::writeUserToFile(newUser, password);
        currentUser = newUser;

        return true;
    }

    // Login user
    static bool login(const string &password)
    {
        // Try to decrypt and read user file with provided password
        User storedUser = FileHandler::readUserFromFile(password);

        // If decryption succeeded and we got a valid user, check if password matches
        // (If password was wrong, readUserFromFile returns empty user)
        if (!storedUser.getUsername().empty() && storedUser.getPassword() == password)
        {
            currentUser = storedUser;
            return true;
        }

        return false;
    }

    // Get current user
    static User getCurrentUser()
    {
        return currentUser;
    }

private:
    static inline User currentUser;
};
