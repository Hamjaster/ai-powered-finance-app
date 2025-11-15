#include <string>
#include "../json.hpp"

using json = nlohmann::json;
using namespace std;

class User
{
private:
  string username;
  string password;

public:
  // Constructors
  User() : username(""), password("") {}

  User(const string &username, const string &password))
      : username(username), password(password) {}

  // Getters
  string getUsername() const { return username; }
  string getPassword() const { return password; }

  // Setters
  void setUsername(const string &username) { this->username = username; }
  void setPassword(const string &password) { this->password = password; }

  // -------- JSON Conversion (same API, but clean) --------

  string toJson() const
  {
    json j;
    j["username"] = username;
    j["password"] = password;
    return j.dump(4); // pretty-printed JSON
  }

  static User fromJson(const string &jsonStr)
  {
    json j = json::parse(jsonStr);

    User user;
    user.username = j.value("username", "");
    user.password = j.value("password", "");

    return user;
  }
};
