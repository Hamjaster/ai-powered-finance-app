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

// Draw a beautiful header with navigation hints
inline void drawScreenHeader(const std::string &title, bool showBack = true) {
  std::string nav = showBack ? "[b]ack  [q]uit" : "[q]uit";
  drawDashedBox(title, nav);
}

// Draw the persistent navigation footer
inline void drawNavFooter() {
  std::cout << std::endl;
  std::cout << "  ";
  for (int i = 0; i < 76; i++) std::cout << "─";
  std::cout << std::endl;
  std::cout << "  ";
  setColor(8); // Gray
  std::cout << "Navigation: ";
  resetColor();
  setColor(11);
  std::cout << "[b]";
  resetColor();
  std::cout << " Back  ";
  setColor(11);
  std::cout << "[m]";
  resetColor();
  std::cout << " Menu  ";
  setColor(11);
  std::cout << "[q]";
  resetColor();
  std::cout << " Quit" << std::endl;
}

// Draw a section title with decoration
inline void drawSectionTitle(const std::string &title, const std::string &icon = "") {
  std::cout << std::endl;
  std::cout << "  ";
  if (!icon.empty()) std::cout << icon << " ";
  setColor(14); // Yellow
  std::cout << title;
  resetColor();
  std::cout << std::endl;
  std::cout << "  ";
  for (int i = 0; i < 50; i++) std::cout << "─";
  std::cout << std::endl;
}

// Draw a styled menu option
inline void drawMenuOption(const std::string &key, const std::string &label, const std::string &icon = "") {
  std::cout << "  ";
  if (!icon.empty()) std::cout << icon << " ";
  setColor(11); // Cyan
  std::cout << "[" << key << "]";
  resetColor();
  std::cout << " " << label << std::endl;
}

// Draw an info line with icon
inline void drawInfoLine(const std::string &icon, const std::string &label, const std::string &value, int color = 7) {
  std::cout << "  " << icon << " " << label << ": ";
  setColor(color);
  std::cout << value << std::endl;
  resetColor();
}

// Draw a status message (success, error, warning)
inline void drawStatusMessage(const std::string &message, const std::string &type = "info") {
  std::cout << "  ";
  if (type == "success") {
    setColor(10); // Green
    std::cout << "✓ ";
  } else if (type == "error") {
    setColor(12); // Red
    std::cout << "✗ ";
  } else if (type == "warning") {
    setColor(14); // Yellow
    std::cout << "⚠ ";
  } else {
    setColor(11); // Cyan
    std::cout << "ℹ ";
  }
  std::cout << message << std::endl;
  resetColor();
}

// Draw a prompt for input
inline void drawPrompt(const std::string &prompt) {
  std::cout << std::endl;
  std::cout << "  ";
  setColor(11);
  std::cout << "▶ ";
  resetColor();
  std::cout << prompt << ": ";
}

// Handle common navigation commands, returns true if handled
inline bool handleNavigation(const std::string &input) {
  if (input == "q" || input == "Q" || input == "quit" || input == "exit") {
    clearScreen();
    std::cout << std::endl;
    std::cout << "  ╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "  ║  Thank you for using AI Expense Manager  ║" << std::endl;
    std::cout << "  ║           See you next time! 👋          ║" << std::endl;
    std::cout << "  ╚══════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    std::exit(0);
    return true;
  }
  return false;
}

