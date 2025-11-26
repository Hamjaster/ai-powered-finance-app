#pragma once

#include "../modules/AI.h"
#include "ScreenRoutes.h"
#include "ScreenUtils.h"
#include <iostream>
#include <string>

inline void showAIScreen() {
  clearScreen();
  drawBoxedTitle("AI Financial Advisor");
  drawSeparator();
  std::cout << "  Type 'exit' to return to the main menu." << std::endl;
  drawSeparator();
  std::cout << std::endl;

  json conversation_history = json::array();

  // System prompt
  conversation_history.push_back(
      {{"role", "system"},
       {"content", "You are a helpful financial advisor. Provide sound "
                   "financial advice and tips."}});

  // Clear buffer if needed, but be careful not to block if empty
  // std::cin.ignore();

  while (true) {
    setColor(11); // Cyan for user
    std::cout << "You: ";
    resetColor();

    std::string user_input;
    std::getline(std::cin, user_input);

    if (user_input == "exit" || user_input == "quit") {
      break;
    }

    if (user_input.empty())
      continue;

    std::cout << "\n";
    setColor(14); // Yellow for thinking
    std::cout << "AI is thinking..." << std::endl;
    resetColor();

    std::string response = AIModule::chat(user_input, conversation_history);

    // Clear the "Thinking..." line (move up and clear line)
    // \033[A moves cursor up 1 line, \033[2K clears the line
    // Note: This might not work in standard Windows cmd without enabling VT
    // processing, but let's try or just print over. For safety in basic cmd,
    // we'll just print the response below.

    std::cout << "\r"; // Return to start of line
    // Overwrite "AI is thinking..." with spaces if we were on the same line,
    // but we did endl. So let's just print the response.

    setColor(10); // Green for AI
    std::cout << "AI: " << response << "\n" << std::endl;
    resetColor();

    drawSeparator();
  }

  showMainMenu();
}
