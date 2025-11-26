#include "nlohmann/json.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

// Helper function to execute shell command and get output
std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

// Helper to escape JSON for command line
std::string escape_json_for_cmd(const std::string &json_str) {
  std::string escaped;
  for (char c : json_str) {
    if (c == '"') {
      escaped += "\\\"";
    } else if (c == '\\') {
      escaped += "\\\\"; // Double escape backslashes
    } else {
      escaped += c;
    }
  }
  return escaped;
}

int main() {
  const std::string api_key =
      "sk-or-v1-"
      "338cd2242baa61a1b2ea72f4fcc373c98e96328906c5ea6634cc6017df87c01f";

  json conversation_history = json::array();

  // System prompt (Financial Advisor)
  conversation_history.push_back(
      {{"role", "system"},
       {"content", "You are a helpful financial advisor. Provide sound "
                   "financial advice and tips."}});

  std::cout << "--- OpenRouter CLI Chat (Type 'exit' to quit) ---" << std::endl;

  while (true) {
    std::cout << "\nYou: ";
    std::string user_input;
    std::getline(std::cin, user_input);

    if (user_input == "exit" || user_input == "quit") {
      break;
    }

    if (user_input.empty())
      continue;

    // Add user message to history
    conversation_history.push_back({{"role", "user"}, {"content", user_input}});

    // Prepare JSON payload
    json payload = {
        {"model", "openai/gpt-3.5-turbo"}, // Default model, can be changed
        {"messages", conversation_history}};

    std::string payload_str = payload.dump();

    // Construct curl command
    // Note: Windows cmd escaping is tricky. We'll use a simplified approach
    // by writing the JSON to a temp file to avoid escaping hell,
    // OR try to escape carefully. Writing to file is safer for complex JSON.

    // Let's try writing to a temp file first as it's much more robust on
    // Windows.
    std::string temp_filename = "temp_request.json";
    FILE *temp_file = fopen(temp_filename.c_str(), "w");
    if (temp_file) {
      fprintf(temp_file, "%s", payload_str.c_str());
      fclose(temp_file);
    } else {
      std::cerr << "Failed to create temp file!" << std::endl;
      continue;
    }

    std::string command =
        "curl -s -X POST https://openrouter.ai/api/v1/chat/completions "
        "-H \"Content-Type: application/json\" "
        "-H \"Authorization: Bearer " +
        api_key +
        "\" "
        "-d @" +
        temp_filename;

    std::cout << "AI: Thinking..." << std::endl;

    try {
      std::string response_str = exec(command.c_str());

      // Parse response
      try {
        auto response_json = json::parse(response_str);

        if (response_json.contains("error")) {
          std::cerr << "API Error: " << response_json["error"].dump()
                    << std::endl;
        } else if (response_json.contains("choices") &&
                   !response_json["choices"].empty()) {
          std::string ai_content =
              response_json["choices"][0]["message"]["content"];
          std::cout << "\rAI: " << ai_content << std::endl;

          // Add AI response to history
          conversation_history.push_back(
              {{"role", "assistant"}, {"content", ai_content}});
        } else {
          std::cerr << "Unexpected response format: " << response_str
                    << std::endl;
        }

      } catch (json::parse_error &e) {
        std::cerr << "JSON Parse Error: " << e.what()
                  << "\nResponse: " << response_str << std::endl;
      }

    } catch (std::exception &e) {
      std::cerr << "Error executing request: " << e.what() << std::endl;
    }

    // Cleanup temp file
    remove(temp_filename.c_str());
  }

  return 0;
}
