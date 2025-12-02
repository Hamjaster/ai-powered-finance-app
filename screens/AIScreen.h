#pragma once

#include "../modules/AI.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"
#include <iostream>
#include <string>

inline void showAIScreen() {
  clearScreen();

  // Beautiful header
  drawScreenHeader("AI Expense • Financial Advisor", true);
  std::cout << std::endl;

  // Welcome section
  std::cout << "  ╭────────────────────────────────────────────────────────────────────╮" << std::endl;
  std::cout << "  │  🤖 Welcome to your AI Financial Advisor!                          │" << std::endl;
  std::cout << "  │     Ask me anything about budgeting, saving, or investing.        │" << std::endl;
  std::cout << "  ╰────────────────────────────────────────────────────────────────────╯" << std::endl;
  std::cout << std::endl;

  json conversation_history = json::array();

  // System prompt
  conversation_history.push_back(
      {{"role", "system"},
       {"content", "You are a helpful financial advisor. Provide sound "
                   "financial advice and tips. Keep responses concise but helpful."}});

  while (true) {
    // Draw the navigation footer each iteration
    std::cout << "  ";
    for (int i = 0; i < 70; i++) std::cout << "─";
    std::cout << std::endl;

    setColor(11); // Cyan for user
    std::cout << "  You: ";
    resetColor();

    std::string user_input;
    std::getline(std::cin, user_input);

    // Handle navigation
    if (user_input == "b" || user_input == "B" || user_input == "back" ||
        user_input == "exit" || user_input == "quit" || user_input.empty()) {
      if (user_input == "quit" || user_input == "exit") {
        handleNavigation("q");
      }
      break;
    }
    if (user_input == "m" || user_input == "M" || user_input == "menu") {
      break;
    }
    if (handleNavigation(user_input)) return;

    std::cout << std::endl;
    setColor(8); // Gray for thinking
    std::cout << "  ⏳ AI is thinking..." << std::endl;
    resetColor();

    std::string response = AIModule::chat(user_input, conversation_history);

    // Clear thinking and show response
    std::cout << std::endl;
    setColor(10); // Green for AI
    std::cout << "  🤖 AI: ";
    resetColor();
    std::cout << response << std::endl;
    std::cout << std::endl;
  }

  showMainMenu();
}
