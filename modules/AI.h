#pragma once

#include "../json.hpp" // Assuming json.hpp is in the root directory
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

class AIModule {
public:
  static std::string chat(const std::string &user_input,
                          json &conversation_history) {
    const std::string api_key =
        "sk-or-v1-"
        "3180f57e5c5592a15f18489202b48355dc3b6af08a350c0758ea5263639baa85";

    // Add user message to history
    conversation_history.push_back({{"role", "user"}, {"content", user_input}});

    // Prepare JSON payload
    json payload = {{"model", "openai/gpt-3.5-turbo"}, // Default model
                    {"messages", conversation_history}};

    std::string payload_str = payload.dump();

    // Write to temp file
    std::string temp_filename = "temp_request.json";
    FILE *temp_file = fopen(temp_filename.c_str(), "w");
    if (temp_file) {
      fprintf(temp_file, "%s", payload_str.c_str());
      fclose(temp_file);
    } else {
      return "Error: Failed to create temp file!";
    }

    std::string command =
        "curl -s -X POST https://openrouter.ai/api/v1/chat/completions "
        "-H \"Content-Type: application/json\" "
        "-H \"Authorization: Bearer " +
        api_key +
        "\" "
        "-d @" +
        temp_filename;

    try {
      std::string response_str = exec(command.c_str());

      // Cleanup temp file
      remove(temp_filename.c_str());

      // Parse response
      try {
        auto response_json = json::parse(response_str);

        if (response_json.contains("error")) {
          return "API Error: " + response_json["error"].dump();
        } else if (response_json.contains("choices") &&
                   !response_json["choices"].empty()) {
          std::string ai_content =
              response_json["choices"][0]["message"]["content"];

          // Add AI response to history
          conversation_history.push_back(
              {{"role", "assistant"}, {"content", ai_content}});

          return ai_content;
        } else {
          return "Unexpected response format: " + response_str;
        }

      } catch (json::parse_error &e) {
        return "JSON Parse Error: " + std::string(e.what());
      }

    } catch (std::exception &e) {
      remove(temp_filename.c_str());
      return "Error executing request: " + std::string(e.what());
    }
  }

private:
  static std::string exec(const char *cmd) {
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
};
