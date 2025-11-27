#pragma once

#include <conio.h>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <windows.h> // Must be included before other Windows-dependent headers

inline void clearScreen() { std::system("cls"); }

inline void setColor(int color) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color);
}

inline void resetColor() { setColor(7); }

inline void drawBoxedTitle(const std::string &title) {
  int width = static_cast<int>(title.length()) + 4;
  std::cout << "+";
  for (int i = 0; i < width - 2; i++)
    std::cout << "-";
  std::cout << "+\n";
  std::cout << "| " << title << " |\n";
  std::cout << "+";
  for (int i = 0; i < width - 2; i++)
    std::cout << "-";
  std::cout << "+\n";
}

inline void drawSeparator() {
  std::cout << "----------------------------------------" << std::endl;
}

inline void drawDashedBox(const std::string &title, const std::string &rightContent = "") {
  int totalWidth = 80;
  int titleLen = static_cast<int>(title.length());
  int rightLen = static_cast<int>(rightContent.length());
  int availableWidth = totalWidth - titleLen - rightLen - 6; // Account for "| " and " |"
  
  if (availableWidth < 1) availableWidth = 1;
  
  // Top border (dashed)
  std::cout << "+";
  for (int i = 0; i < totalWidth - 2; i++) std::cout << "-";
  std::cout << "+\n";
  
  // Content line
  std::cout << "| " << title;
  for (int i = 0; i < availableWidth; i++) std::cout << " ";
  std::cout << rightContent << " |\n";
  
  // Bottom border (dashed)
  std::cout << "+";
  for (int i = 0; i < totalWidth - 2; i++) std::cout << "-";
  std::cout << "+\n";
}

inline void drawTableHeader(const std::vector<std::string> &headers, const std::vector<int> &widths) {
  std::cout << "+";
  for (size_t i = 0; i < headers.size(); i++) {
    for (int j = 0; j < widths[i]; j++) std::cout << "-";
    if (i < headers.size() - 1) std::cout << "+";
  }
  std::cout << "+\n";
  
  std::cout << "|";
  for (size_t i = 0; i < headers.size(); i++) {
    std::cout << " " << headers[i];
    int padding = widths[i] - static_cast<int>(headers[i].length()) - 1;
    for (int j = 0; j < padding; j++) std::cout << " ";
    std::cout << "|";
  }
  std::cout << "\n";
  
  std::cout << "+";
  for (size_t i = 0; i < headers.size(); i++) {
    for (int j = 0; j < widths[i]; j++) std::cout << "-";
    if (i < headers.size() - 1) std::cout << "+";
  }
  std::cout << "+\n";
}

inline void drawTableRow(const std::vector<std::string> &cells, const std::vector<int> &widths) {
  std::cout << "|";
  for (size_t i = 0; i < cells.size(); i++) {
    std::cout << " " << cells[i];
    int padding = widths[i] - static_cast<int>(cells[i].length()) - 1;
    for (int j = 0; j < padding; j++) std::cout << " ";
    std::cout << "|";
  }
  std::cout << "\n";
}

inline void drawTableBottom(const std::vector<int> &widths) {
  std::cout << "+";
  for (size_t i = 0; i < widths.size(); i++) {
    for (int j = 0; j < widths[i]; j++) std::cout << "-";
    if (i < widths.size() - 1) std::cout << "+";
  }
  std::cout << "+\n";
}

inline void setColorMagenta() {
  setColor(13); // Magenta/Pink
}

inline std::string getCurrentMonthYear() {
  time_t now = time(0);
  tm *timeinfo = localtime(&now);
  
  const char *months[] = {"January", "February", "March", "April", "May", "June",
                          "July", "August", "September", "October", "November", "December"};
  
  return std::string(months[timeinfo->tm_mon]) + " " + std::to_string(1900 + timeinfo->tm_year);
}

inline std::string getPasswordInput() {
  std::string password;
  char ch;

  while (true) {
    ch = _getch();

    if (ch == '\r' || ch == '\n') {
      std::cout << std::endl;
      break;
    } else if (ch == '\b' || ch == 127) {
      if (!password.empty()) {
        password.pop_back();
        std::cout << "\b \b";
      }
    } else {
      password += ch;
      std::cout << "*";
    }
  }

  return password;
}

inline std::string getInput() {
  std::string input;
  std::getline(std::cin, input);
  if (input.empty() && std::cin.peek() == '\n') {
    std::cin.ignore();
  }
  return input;
}

inline double getDoubleInput() {
  std::string input = getInput();
  try {
    return std::stod(input);
  } catch (...) {
    return -1.0;
  }
}
