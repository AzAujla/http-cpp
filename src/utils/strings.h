#pragma once
#include <string>

inline std::string sanitize(const std::string &input) {
  std::string out;
  out.reserve(input.size());
  for (char c : input) {
    if (c != '\r' && c != '\n')
      out += c;
  }
  return out;
}

inline std::string sanitize_cookie_key(const std::string &input) {
  std::string out;
  for (char c : input) {
    if (c != '\r' && c != '\n' && c != '=' && c != ';')
      out += c;
  }
  return out;
}

inline std::string url_decode(const std::string &input) {
  std::string out;
  for (size_t i = 0; i < input.size(); i++) {
    if (input[i] == '+') {
      out += ' ';
    } else if (input[i] == '%' && i + 2 < input.size()) {
      int hex = std::stoi(input.substr(i + 1, 2), nullptr, 16);
      out += static_cast<char>(hex);
      i += 2;
    } else {
      out += input[i];
    }
  }
  return out;
}
