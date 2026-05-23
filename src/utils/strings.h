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
