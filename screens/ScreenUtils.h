#pragma once

// IMPORTANT: Include windows.h BEFORE C++ standard headers to avoid std::byte conflict
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ═══════════════════════════════════════════════════════════════════════════
// CORE UTILITIES
// ═══════════════════════════════════════════════════════════════════════════

inline void clearScreen() { std::system("cls"); }

inline void setColor(int color) {
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color);
}

inline void resetColor() { setColor(7); }

// Color constants for consistency
const int COLOR_WHITE = 7;
const int COLOR_GRAY = 8;
const int COLOR_GREEN = 10;
const int COLOR_CYAN = 11;
const int COLOR_RED = 12;
const int COLOR_MAGENTA = 13;
const int COLOR_YELLOW = 14;
const int COLOR_BRIGHT_WHITE = 15;

// Standard box width (inner content width)
// Header: ╔ + BOX_WIDTH × ═ + ╗
// Content: ║ + space + BOX_INNER text + space + ║ where BOX_INNER = BOX_WIDTH - 2
const int BOX_WIDTH = 76;
const int BOX_INNER = 74;  // BOX_WIDTH - 2 for the two spaces

// ═══════════════════════════════════════════════════════════════════════════
// BOX DRAWING - Consistent Unicode boxes
// ═══════════════════════════════════════════════════════════════════════════

// Draw a double-line header box
inline void drawHeader(const std::string& title, const std::string& nav = "") {
  std::cout << "  ╔";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "═";
  std::cout << "╗" << std::endl;

  int titleLen = static_cast<int>(title.length());
  int navLen = static_cast<int>(nav.length());
  int textUsed = titleLen + navLen;
  int padding = BOX_INNER - textUsed;
  if (padding < 1) padding = 1;

  std::cout << "  ║ ";
  setColor(COLOR_CYAN);
  std::cout << title;
  resetColor();
  for (int i = 0; i < padding; i++) std::cout << " ";
  setColor(COLOR_GRAY);
  std::cout << nav;
  resetColor();
  std::cout << " ║" << std::endl;

  std::cout << "  ╚";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "═";
  std::cout << "╝" << std::endl;
}

// Draw screen header with navigation hints
inline void drawScreenHeader(const std::string &title, bool showBack = true) {
  std::string nav = showBack ? "[b]ack  [q]uit" : "[q]uit";
  drawHeader(title, nav);
}

// Legacy compatibility
inline void drawDashedBox(const std::string &title, const std::string &rightContent = "") {
  drawHeader(title, rightContent);
}

inline void drawBoxedTitle(const std::string &title) {
  drawHeader(title);
}

// Draw a simple line separator
inline void drawLine(char c = '-') {
  std::cout << "  ";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << c;
  std::cout << std::endl;
}

inline void drawSeparator() {
  drawLine('-');
}

// Draw thin box (for content sections)
inline void drawThinBox(const std::vector<std::string>& lines) {
  std::cout << "  ┌";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┐" << std::endl;

  for (const auto& line : lines) {
    int padding = BOX_INNER - static_cast<int>(line.length());
    if (padding < 0) padding = 0;
    std::cout << "  │ " << line;
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << " │" << std::endl;
  }

  std::cout << "  └";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┘" << std::endl;
}

// Draw info box with title and description
inline void drawInfoBox(const std::string& line1, const std::string& line2 = "", const std::string& line3 = "") {
  std::cout << "  ┌";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┐" << std::endl;
  
  auto printLine = [](const std::string& text) {
    int padding = BOX_INNER - static_cast<int>(text.length());
    if (padding < 0) padding = 0;
    std::cout << "  │ " << text;
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << " │" << std::endl;
  };
  
  printLine(line1);
  if (!line2.empty()) printLine(line2);
  if (!line3.empty()) printLine(line3);
  
  std::cout << "  └";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┘" << std::endl;
}

// Draw info box with divider
inline void drawInfoBoxWithDivider(const std::string& title, const std::vector<std::string>& items) {
  std::cout << "  ┌";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┐" << std::endl;
  
  // Title line
  int titlePad = BOX_INNER - static_cast<int>(title.length());
  if (titlePad < 0) titlePad = 0;
  std::cout << "  │ " << title;
  for (int i = 0; i < titlePad; i++) std::cout << " ";
  std::cout << " │" << std::endl;
  
  // Divider
  std::cout << "  ├";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┤" << std::endl;
  
  // Items
  for (const auto& item : items) {
    int itemPad = BOX_INNER - static_cast<int>(item.length());
    if (itemPad < 0) itemPad = 0;
    std::cout << "  │ " << item;
    for (int i = 0; i < itemPad; i++) std::cout << " ";
    std::cout << " │" << std::endl;
  }
  
  std::cout << "  └";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << "┘" << std::endl;
}

// ═══════════════════════════════════════════════════════════════════════════
// TABLE DRAWING - Unicode tables
// ═══════════════════════════════════════════════════════════════════════════

inline void drawTableHeader(const std::vector<std::string> &headers, const std::vector<int> &widths) {
  std::cout << "  ┌";
  for (size_t i = 0; i < headers.size(); i++) {
    for (int j = 0; j < widths[i]; j++) std::cout << "─";
    if (i < headers.size() - 1) std::cout << "┬";
  }
  std::cout << "┐" << std::endl;
  
  std::cout << "  │";
  for (size_t i = 0; i < headers.size(); i++) {
    setColor(COLOR_YELLOW);
    std::cout << " " << headers[i];
    resetColor();
    int padding = widths[i] - static_cast<int>(headers[i].length()) - 1;
    for (int j = 0; j < padding; j++) std::cout << " ";
    std::cout << "│";
  }
  std::cout << std::endl;
  
  std::cout << "  ├";
  for (size_t i = 0; i < headers.size(); i++) {
    for (int j = 0; j < widths[i]; j++) std::cout << "─";
    if (i < headers.size() - 1) std::cout << "┼";
  }
  std::cout << "┤" << std::endl;
}

inline void drawTableRow(const std::vector<std::string> &cells, const std::vector<int> &widths) {
  std::cout << "  │";
  for (size_t i = 0; i < cells.size(); i++) {
    std::cout << " " << cells[i];
    int padding = widths[i] - static_cast<int>(cells[i].length()) - 1;
    for (int j = 0; j < padding; j++) std::cout << " ";
    std::cout << "│";
  }
  std::cout << std::endl;
}

inline void drawTableBottom(const std::vector<int> &widths) {
  std::cout << "  └";
  for (size_t i = 0; i < widths.size(); i++) {
    for (int j = 0; j < widths[i]; j++) std::cout << "─";
    if (i < widths.size() - 1) std::cout << "┴";
  }
  std::cout << "┘" << std::endl;
}

// ═══════════════════════════════════════════════════════════════════════════
// PROGRESS BAR
// ═══════════════════════════════════════════════════════════════════════════

inline std::string drawProgressBar(double percent, int width = 20) {
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;

  int filled = static_cast<int>((percent / 100.0) * width);
  if (filled > width) filled = width;

  std::ostringstream oss;
  oss << "[";
  for (int i = 0; i < filled; i++) oss << "█";
  for (int i = filled; i < width; i++) oss << "░";
  oss << "]";
  return oss.str();
}

inline void drawColoredProgressBar(double percent, int width = 20) {
  if (percent < 0) percent = 0;

  int filled = static_cast<int>((std::min(percent, 100.0) / 100.0) * width);
  if (filled > width) filled = width;

  // Choose color based on percent
  int color = COLOR_GREEN;
  if (percent >= 90) color = COLOR_RED;
  else if (percent >= 75) color = COLOR_YELLOW;

  std::cout << "[";
  setColor(color);
  for (int i = 0; i < filled; i++) std::cout << "█";
  resetColor();
  for (int i = filled; i < width; i++) std::cout << "░";
  std::cout << "]";
}

// ═══════════════════════════════════════════════════════════════════════════
// SECTION TITLES & MENU OPTIONS
// ═══════════════════════════════════════════════════════════════════════════

inline void drawSectionTitle(const std::string &title, const std::string &icon = "") {
  std::cout << std::endl;
  std::cout << "  ";
  if (!icon.empty()) std::cout << icon << " ";
  setColor(COLOR_YELLOW);
  std::cout << title;
  resetColor();
  std::cout << std::endl;
  std::cout << "  ";
  for (int i = 0; i < 50; i++) std::cout << "─";
  std::cout << std::endl;
}

inline void drawMenuOption(const std::string &key, const std::string &label, const std::string &icon = "") {
  std::cout << "  ";
  if (!icon.empty()) std::cout << icon << " ";
  setColor(COLOR_CYAN);
  std::cout << "[" << key << "]";
  resetColor();
  std::cout << " " << label << std::endl;
}

inline void drawInfoLine(const std::string &icon, const std::string &label, const std::string &value, int color = COLOR_WHITE) {
  std::cout << "  " << icon << " " << label << ": ";
  setColor(color);
  std::cout << value;
  resetColor();
  std::cout << std::endl;
}

inline void setColorMagenta() {
  setColor(COLOR_MAGENTA);
}

// ═══════════════════════════════════════════════════════════════════════════
// NAVIGATION & PROMPTS
// ═══════════════════════════════════════════════════════════════════════════

inline void drawNavFooter() {
  std::cout << std::endl;
  std::cout << "  ";
  for (int i = 0; i < BOX_WIDTH; i++) std::cout << "─";
  std::cout << std::endl;
  std::cout << "  ";
  setColor(COLOR_GRAY);
  std::cout << "Navigation: ";
  resetColor();
  setColor(COLOR_CYAN);
  std::cout << "[b]";
  resetColor();
  std::cout << " Back  ";
  setColor(COLOR_CYAN);
  std::cout << "[m]";
  resetColor();
  std::cout << " Menu  ";
  setColor(COLOR_CYAN);
  std::cout << "[q]";
  resetColor();
  std::cout << " Quit" << std::endl;
}

inline void drawPrompt(const std::string &prompt) {
  std::cout << std::endl;
  std::cout << "  ";
  setColor(COLOR_CYAN);
  std::cout << "► ";
  resetColor();
  std::cout << prompt << ": ";
}

inline void drawQuickAddHint() {
  std::cout << "  ";
  setColor(COLOR_GRAY);
  std::cout << "Quick: Type '+50 lunch' for income or '-30 groceries' for expense";
  resetColor();
  std::cout << std::endl;
}

// ═══════════════════════════════════════════════════════════════════════════
// STATUS MESSAGES
// ═══════════════════════════════════════════════════════════════════════════

inline void drawStatusMessage(const std::string &message, const std::string &type = "info") {
  std::cout << "  ";
  if (type == "success") {
    setColor(COLOR_GREEN);
    std::cout << "✓ ";
  } else if (type == "error") {
    setColor(COLOR_RED);
    std::cout << "✗ ";
  } else if (type == "warning") {
    setColor(COLOR_YELLOW);
    std::cout << "⚠ ";
  } else {
    setColor(COLOR_CYAN);
    std::cout << "ℹ ";
  }
  std::cout << message;
  resetColor();
  std::cout << std::endl;
}

inline void drawSuccessBox(const std::string& message) {
  int msgLen = static_cast<int>(message.length());
  int boxWidth = std::max(52, msgLen + 8);  // Min 52, or message + padding
  
  std::cout << "  ┌";
  for (int i = 0; i < boxWidth - 2; i++) std::cout << "─";
  std::cout << "┐" << std::endl;
  
  std::cout << "  │  ";
  setColor(COLOR_GREEN);
  std::cout << "✓ " << message;
  resetColor();
  int padding = boxWidth - 6 - msgLen;
  if (padding < 0) padding = 0;
  for (int i = 0; i < padding; i++) std::cout << " ";
  std::cout << "│" << std::endl;
  
  std::cout << "  └";
  for (int i = 0; i < boxWidth - 2; i++) std::cout << "─";
  std::cout << "┘" << std::endl;
}

inline void drawErrorBox(const std::string& message) {
  int msgLen = static_cast<int>(message.length());
  int boxWidth = std::max(52, msgLen + 8);  // Min 52, or message + padding
  
  std::cout << "  ┌";
  for (int i = 0; i < boxWidth - 2; i++) std::cout << "─";
  std::cout << "┐" << std::endl;
  
  std::cout << "  │  ";
  setColor(COLOR_RED);
  std::cout << "✗ " << message;
  resetColor();
  int padding = boxWidth - 6 - msgLen;
  if (padding < 0) padding = 0;
  for (int i = 0; i < padding; i++) std::cout << " ";
  std::cout << "│" << std::endl;
  
  std::cout << "  └";
  for (int i = 0; i < boxWidth - 2; i++) std::cout << "─";
  std::cout << "┘" << std::endl;
}

// ═══════════════════════════════════════════════════════════════════════════
// INPUT UTILITIES
// ═══════════════════════════════════════════════════════════════════════════

inline std::string getInput() {
  std::string input;
  std::getline(std::cin, input);
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

// ═══════════════════════════════════════════════════════════════════════════
// NAVIGATION HANDLING
// ═══════════════════════════════════════════════════════════════════════════

inline bool handleNavigation(const std::string &input) {
  if (input == "q" || input == "Q" || input == "quit" || input == "exit") {
    clearScreen();
    std::cout << std::endl;
    std::cout << "  ╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "  ║                                                ║" << std::endl;
    std::cout << "  ║   Thank you for using AI Expense Manager! 👋   ║" << std::endl;
    std::cout << "  ║         See you next time!                     ║" << std::endl;
    std::cout << "  ║                                                ║" << std::endl;
    std::cout << "  ╚════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    std::exit(0);
    return true;
  }
  return false;
}

// ═══════════════════════════════════════════════════════════════════════════
// DATE & FORMAT UTILITIES
// ═══════════════════════════════════════════════════════════════════════════

inline std::string getCurrentMonthYear() {
  time_t now = time(0);
  tm *timeinfo = localtime(&now);
  
  const char *months[] = {"January", "February", "March", "April", "May", "June",
                          "July", "August", "September", "October", "November", "December"};
  
  return std::string(months[timeinfo->tm_mon]) + " " + std::to_string(1900 + timeinfo->tm_year);
}

inline std::string formatCurrency(double amount) {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2) << amount;
  std::string str = oss.str();
  
  int pos = static_cast<int>(str.find('.'));
  if (pos == std::string::npos) pos = static_cast<int>(str.length());
  
  for (int i = pos - 3; i > 0; i -= 3) {
    str.insert(i, ",");
  }
  
  return "$" + str;
}

// ═══════════════════════════════════════════════════════════════════════════
// QUICK ADD PARSING
// ═══════════════════════════════════════════════════════════════════════════

struct QuickAddResult {
  bool valid;
  std::string type;
  double amount;
  std::string description;
};

inline QuickAddResult parseQuickAdd(const std::string& input) {
  QuickAddResult result = {false, "", 0, ""};
  
  if (input.empty()) return result;
  
  // Format: "+50 lunch" or "-30 groceries"
  char sign = input[0];
  if (sign != '+' && sign != '-') return result;
  
  // Find space between amount and description
  size_t spacePos = input.find(' ');
  if (spacePos == std::string::npos || spacePos < 2) return result;
  
  // Parse amount
  std::string amountStr = input.substr(1, spacePos - 1);
  try {
    result.amount = std::stod(amountStr);
  } catch (...) {
    return result;
  }
  
  if (result.amount <= 0) return result;
  
  // Parse description
  result.description = input.substr(spacePos + 1);
  if (result.description.empty()) return result;
  
  result.type = (sign == '+') ? "income" : "expense";
  result.valid = true;
  
  return result;
}

