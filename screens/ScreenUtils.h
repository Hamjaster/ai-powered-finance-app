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
