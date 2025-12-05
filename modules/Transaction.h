#pragma once
#include "../include/nlohmann/json.hpp"
#include <ctime>
#include <string>

using json = nlohmann::json;
using namespace std;

class Transaction
{
private:
  int id;
  double amount;
  string type; // "income" or "expense"
  string description;
  string date; // Format: "15 Nov, 25"

public:
  // Constructors
  Transaction() : id(0), type(""), amount(0.0), description(""), date("") {}

  Transaction(const string &type, double amount, const string &description)
      : id(0), type(type), amount(amount), description(description), date("")
  {
    this->date = generateCurrentDate();
  }

  Transaction(int id, const string &type, double amount,
              const string &description, const string &date)
      : id(id), type(type), amount(amount), description(description),
        date(date) {}

  // Getters
  int getId() const { return id; }
  string getType() const { return type; }
  double getAmount() const { return amount; }
  string getDescription() const { return description; }
  string getDate() const { return date; }

  // Setters
  void setId(int id) { this->id = id; }
  void setType(const string &type) { this->type = type; }
  void setAmount(double amount) { this->amount = amount; }
  void setDescription(const string &description)
  {
    this->description = description;
  }
  void setDate(const string &date) { this->date = date; }

  // Generate current date in format "15 Nov, 25"
  static string generateCurrentDate()
  {
    time_t now = time(0);
    tm *timeinfo = localtime(&now);

    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    int day = timeinfo->tm_mday;
    int month = timeinfo->tm_mon;
    int year = timeinfo->tm_year % 100; // Last 2 digits of year

    string dateStr =
        to_string(day) + " " + string(months[month]) + ", " + to_string(year);
    return dateStr;
  }

  // JSON conversion
  json toJson() const
  {
    json j;
    j["id"] = id;
    j["type"] = type;
    j["amount"] = amount;
    j["description"] = description;
    j["date"] = date;
    return j;
  }

  static Transaction fromJson(const json &j)
  {
    Transaction t;
    t.id = j.value("id", 0);
    t.type = j.value("type", "");
    t.amount = j.value("amount", 0.0);
    t.description = j.value("description", "");
    t.date = j.value("date", "");
    return t;
  }
};
