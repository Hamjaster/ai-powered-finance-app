#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> // Required for std::setw
#include <curl/curl.h>
#include "json.hpp" // For parsing JSON

// Include windows.h only if compiling on Windows
#ifdef _WIN32
#include <windows.h>
#endif

// Use nlohmann/json
using json = nlohmann::json;

// Callback function to capture CURL output
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t total = size * nmemb;
    out->append((char*)contents, total);
    return total;
}

// Function to escape special characters for JSON
std::string escape_json_string(const std::string& input) {
    std::ostringstream ss;
    for (char c : input) {
        switch (c) {
            case '"':  ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b";  break;
            case '\f': ss << "\\f";  break;
            case '\n': ss << "\\n";  break;
            case '\r': ss << "\\r";  break;
            case '\t': ss << "\\t";  break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    // Handle other control characters
                    ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                } else {
                    ss << c;
                }
        }
    }
    return ss.str();
}

int main() {
    // Fix for displaying UTF-8 characters in the Windows console
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    // --- IMPORTANT ---
    // Replace this with your actual OpenRouter API key
    std::string api_key = "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";
    // -----------------

    std::string prompt;
    std::cout << "Enter prompt: ";
    std::getline(std::cin, prompt);

    // Escape the prompt string to make it safe for JSON
    std::string escaped_prompt = escape_json_string(prompt);

    // Build JSON with the correct model and escaped prompt
    // (FIXED typo here: was std{::string)
    std::string json_body =
        "{"
        "\"model\": \"openai/gpt-oss-20b\","
        "\"messages\": [{\"role\": \"user\", \"content\": \"" + escaped_prompt + "\"}]"
        "}";

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL\n";
        return 1;
    }

    std::string response;
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://openrouter.ai/api/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
    } else {
        // Parse the JSON response
        try {
            auto j = json::parse(response);

            // Check if the expected fields exist
            if (j.contains("choices") && j["choices"].is_array() && !j["choices"].empty() &&
                j["choices"][0].contains("message") && j["choices"][0]["message"].contains("content"))
            {
                // Extract and print the clean text content
                std::string message_content = j["choices"][0]["message"]["content"];
                std::cout << "\n" << message_content << "\n";
            } else if (j.contains("error")) {
                // Print the API error message
                std::string error_message = j["error"]["message"];
                std::cerr << "\nAPI Error: " << error_message << "\n";
            }
            else {
                std::cerr << "\nUnexpected JSON structure:\n" << response << "\n";
            }
        } catch (json::parse_error& e) {
            // Catch parsing errors (e.g., if the response isn't valid JSON)
            std::cerr << "\nFailed to parse JSON response: " << e.what() << "\n";
            std::cerr << "Raw response: " << response << "\n";
        }
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return 0;
}